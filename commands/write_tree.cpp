#include "write_tree.h"
#include <iostream>

void WriteTreeCommand::execute(const std::vector<std::string>& args, Repository& repo) {
    try {
        std::string tree_sha = repo.writeTree(repo.getWorktreeDir());
        std::cout << tree_sha << "\n";
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }
}