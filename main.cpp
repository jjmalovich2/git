#include <iostream>
#include <unordered_map>
#include <map>
#include <string>
#include <functional>
#include <vector>
#include <memory>

#include "utils/HelpText.h"
#include "utils/Repository.h"

#include "commands/init.h"
#include "commands/help.h"
#include "commands/hash_object.h"
#include "commands/cat.h"
#include "commands/ls_tree.h"
#include "commands/write_tree.h"
#include "commands/commit.h"

enum CommandType { INIT, ADD, HELP, UNKNOWN };

CommandType resolve(const std::string& cmd) {
    if (cmd == "init") return INIT;
    if (cmd == "add") return ADD;
    if (cmd == "help") return HELP;
    return UNKNOWN;
}

int main(int argc, char* argv[]) {
    std::vector<std::string> args(argv, argv + argc);

    if (argc < 2) {
        std::cout << "These are the common JGit commands you can use: \n\n";
        for (const auto& pair : help_reg) {
            std::cout << pair.first << "\t" << pair.second.summary << "\n";
        }
        return 0;
    }

    

    std::unordered_map<std::string, std::unique_ptr<Command>> command_reg;

    command_reg["init"] = std::make_unique<InitCommand>();
    command_reg["help"] = std::make_unique<HelpCommand>();
    command_reg["hash-object"] = std::make_unique<HashObjectCommand>();
    command_reg["cat"] = std::make_unique<CatCommand>();
    command_reg["ls-tree"] = std::make_unique<LsTreeCommand>();
    command_reg["write-tree"] = std::make_unique<WriteTreeCommand>();
    command_reg["commit"] = std::make_unique<CommitCommand>();

    std::string cmd = args[1];

    Repository repo(".", (cmd == "init"));

    if (command_reg.find(cmd) != command_reg.end()) {
        command_reg[cmd]->execute(args, repo);
    } else {
        std::cerr << "git: '" << cmd << "' is not a command\n";
        return 1;
    }

    return 0;
}