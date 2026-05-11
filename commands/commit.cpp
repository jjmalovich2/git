#include "commit.h"
#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>

void CommitCommand::execute(const std::vector<std::string>& args, Repository& repo) {
    // jgit commit [-p <parent_sha>] -m "message"

    std::string parent_sha = "";
    std::string message = "";

    for (size_t i = 2; i < args.size(); ++i) {
        if (args[i] == "-p" && i + 1 < args.size()) {
            parent_sha = args[i + 1];
            i++;
        } else if (args[i] == "-m" && i + 1 < args.size()) {
            message = args[i + 1];
            i++;
        }
    }

    if (message.empty()) {
        std::cerr << "Fatal: commit message cannot be empty\n";
        return;
    }

    try {
        // create master
        std::string tree_sha = repo.writeTree(repo.getWorktreeDir());

        auto now = std::chrono::system_clock::now();
        std::time_t time_now = std::chrono::system_clock::to_time_t(now);
        std::tm* local_time = std::localtime(&time_now);

        char tz_buffer[10];
        std::strftime(tz_buffer, sizeof(tz_buffer), "%z", local_time);

        std::string timestamp = std::to_string(time_now) + " " + tz_buffer;
        std::string author_line = "JGit User <user@example.com> " + timestamp;

        std::ostringstream commit_content;
        commit_content << "tree " << tree_sha << "\n";

        if (!parent_sha.empty()) {
            commit_content << "parent " << parent_sha << "\n";
        }

        commit_content << "author " << author_line << "\n";
        commit_content << "committer " << author_line << "\n";
        commit_content << "\n"; // required blank line
        commit_content << message << "\n";

        // hash and write to db
        std::string commit_sha = repo.writeObject(commit_content.str(), "commit");
        std::cout << "[master (root-commit) " << commit_sha.substr(0, 7) << "] " << message << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error during commit: " << e.what() << "\n";
    }
}