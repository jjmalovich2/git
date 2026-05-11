#pragma once
#include <string>
#include <filesystem>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "../libs/sha1.h"

inline std::string generate_sha1(const std::string& data) {
    SHA1 hasher;
    return hasher(data);
}

inline std::string readFile(const std::filesystem::path& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file) throw std::runtime_error("Fatal: could not open file " + filepath.string());

    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

inline std::string hex_to_bytes(const std::string& hex) {
    std::string bytes;
    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        char byte = (char) strtol(byteString.c_str(), nullptr, 16);
        bytes.push_back(byte);
    }

    return bytes;
}

class Repository {
private:
    std::filesystem::path worktree_dir;
    std::filesystem::path git_dir;

    std::filesystem::path find_git_root(std::filesystem::path current) {
        current = std::filesystem::absolute(current);

        while (true) {
            if (std::filesystem::exists(current / ".jgit")) {
                return current;
            }

            std::filesystem::path parent = current.parent_path();

            if (parent == current) {
                return "";
            }

            current = parent;
        }
    }

public:
    Repository(const std::filesystem::path& worktree, bool force_current_dir) {
        worktree_dir = (force_current_dir) ? std::filesystem::absolute(worktree) : find_git_root(worktree);

        if (!worktree_dir.empty()) {
            git_dir = worktree_dir / ".jgit";
        } else {
            worktree_dir = std::filesystem::absolute(worktree);
            git_dir = worktree_dir / ".jgit";
        }
    }

    std::string writeObject(const std::string& data, const std::string& type = "blob") {
        // build blob then hash
        std::string header = type + " " + std::to_string(data.size());
        std::string store = header + '\0' + data;
        std::string hash = generate_sha1(store);

        // 2/38 split paths
        std::string dir_name = hash.substr(0, 2);
        std::string file_name = hash.substr(2);

        std::filesystem::path object_dir = git_dir / "objects" / dir_name;
        std::filesystem::path object_path = object_dir / file_name;

        // save to disk
        if (!std::filesystem::exists(object_path)) {
            std::filesystem::create_directories(object_dir);

            std::ofstream out(object_path, std::ios::binary);
            if (out.is_open()) {
                out.write(store.c_str(), store.size());
                out.close();
            } else {
                throw std::runtime_error("Fatal: could not write object to database");
            }
        }

        return hash;
    }

    std::string readObject(const std::string& hash) {
        if (hash.length() != 40) {
            throw std::runtime_error("Fatal: not a valid 40-char hash");
        }

        std::string dir_name = hash.substr(0, 2);
        std::string file_name = hash.substr(2);
        std::filesystem::path object_path = git_dir / "objects" / dir_name / file_name;

        if (!std::filesystem::exists(object_path)) {
            throw std::runtime_error("Fatal: not a valid object name: " + hash);
        }

        return readFile(object_path);
    }

    std::string writeTree(const std::filesystem::path& dir) {
        std::vector<std::pair<std::string, std::filesystem::path>> entries;

        for (const auto& entry : std::filesystem::directory_iterator(dir)) {
            std::string name = entry.path().filename().string();

            // ignore .jgit and .git
            if (name == ".jgit" || name == ".git") continue;

            entries.push_back({name, entry.path()});
        }

        // sort then build
        std::sort(entries.begin(), entries.end());
        std::string tree_content = "";

        for (const auto& [name, path] : entries) {
            std::string mode;
            std::string sha_hex;

            if (std::filesystem::is_directory(path)) {
                mode = "40000";
                sha_hex = writeTree(path);
            } else {
                mode = "100644";

                // turn into blob
                std::string file_data = readFile(path);
                sha_hex = writeObject(file_data, "blob");
            }

            // convert 40-char hex to 20-byte binary
            std::string sha_bytes = hex_to_bytes(sha_hex);

            tree_content += mode + " " + name + '\0' + sha_bytes;
        }

        return writeObject(tree_content, "tree");
    }

    bool exists() const {
        return std::filesystem::exists(git_dir);
    }

    std::filesystem::path getGitDir() {
        return git_dir;
    }

    std::filesystem::path getWorktreeDir() {
        return worktree_dir;
    }

};