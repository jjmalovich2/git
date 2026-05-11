#include "cat.h"
#include <iostream>

void CatCommand::execute(const std::vector<std::string>& args, Repository& repo) {
    if (args.size() < 4 || args[2] != "-p") {
        std::cerr << "Usage: jgit cat -p <object-hash>\n";
        return;
    }

    std::string hash = args[3];

    try {
        std::string raw_data = repo.readObject(hash);

        size_t null_pos = raw_data.find('\0');
        if (null_pos == std::string::npos) {
            std::cerr << "Fatal: corrupted object file (missing null byte)\n";
            return;
        }

        std::string header = raw_data.substr(0, null_pos);
        std::string content = raw_data.substr(null_pos + 1);
        std::cout << content;
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }
}