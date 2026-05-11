#include "ls_tree.h"
#include <iostream>
#include <string>

void LsTreeCommand::execute(const std::vector<std::string>& args, Repository& repo) {
    if (args.size() < 4 || args[2] != "--name-only") {
        std::cerr << "Usage: jgit ls-tree --name-only <tree_sha>\n";
        return;
    }

    std::string tree_sha = args[3];

    try {
        std::string raw_data = repo.readObject(tree_sha);

        size_t pos = raw_data.find('\0');
        if (pos == std::string::npos) {
            std::cerr << "Fatal: corrupted tree object (no null byte)\n";
            return;
        }
        pos++;

        while (pos < raw_data.size()) {
            size_t space_pos = raw_data.find(' ', pos);
            if (space_pos == std::string::npos) break;

            // skip the space
            pos = space_pos + 1;

            size_t null_pos = raw_data.find('\0', pos);
            if (null_pos == std::string::npos) break;

            std::string filename = raw_data.substr(pos, null_pos - pos);
            std::cout << filename << "\n";

            // skip null byte and 20-byte binary sha-1
            pos = null_pos + 1 + 20;
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }
}