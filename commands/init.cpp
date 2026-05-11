#include <iostream>
#include <filesystem>
#include <fstream>

#include "init.h"
#include "../utils/HelpText.h"

void InitCommand::execute(const std::vector<std::string>& args, Repository& repo) {
    if (args.size() > 2 && (args[2] == "--help" || args[2] == "-h")) {
        std::cout << "Usage: " << help_reg["init"].usage << "\n\n";
        std::cout << help_reg["init"].description << "\n";
        return;
    }

    std::filesystem::path git_dir = repo.getGitDir();

    if (repo.exists()) {
        std::cout << "reinitializing existing Git repository in " << git_dir << "\n\n";
    } else {
        std::cout << "initialized empty Git repository in " << git_dir << "\n\n";
    }

    std::filesystem::create_directories(git_dir / "objects");
    std::filesystem::create_directories(git_dir / "refs" / "heads");

    std::ofstream head_file(git_dir / "HEAD");
    if (head_file.is_open()) {
        head_file << "ref: refs/heads/master\n";
        head_file.close();
    } else {
        std::cerr << "Fatal: Could not create HEAD file\n";
        return;
    }

    std::ofstream config_file(git_dir / "config");
    if (config_file.is_open()) {
        config_file << "[core]\n"
                    << "\trepositoryformatversion = 0\n"
                    << "\tfilemode = true\n"
                    << "\tbare = false\n";
        config_file.close();
    } else {
        std::cerr << " Fatal: Could not create config file\n";
        return;
    }
}