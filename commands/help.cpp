#include <iostream>

#include "../utils/HelpText.h"
#include "help.h"

void HelpCommand::execute(const std::vector<std::string>& args, Repository& repo) {
    if (args.size() > 2) {
        std::string target = args[2];

        if (help_reg.find(target) != help_reg.end()) {
            std::cout << "Usage: " << help_reg[target].usage << "\n\n";
            std::cout << help_reg[target].description << "\n";
        } else {
            std::cout << "Command not found: " << target << "\n";
        }
    } else {
        std::cout << "These are the common Git commands you can use: \n\n";
        for (const auto& pair : help_reg) {
            std::cout << pair.first << "\t" << pair.second.summary << "\n";
        }
    }
}