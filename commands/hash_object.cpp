#include "hash_object.h"
#include <iostream>
#include <filesystem>
#include <string>

#include "../utils/HelpText.h"

void HashObjectCommand::execute(const std::vector<std::string>& args, Repository& repo) {
    bool write_flag = false;
    std::string filepath_str = "";

    for (size_t i = 2; i < args.size(); ++i) {
        if (args[i] == "-w" || args[i] == "-write") {
            write_flag = true;
        } else {
            filepath_str = args[i];
        }
    }

    if (filepath_str.empty()) {
        std::cerr << "Usage: jgit hash-object [-w] <file>\n";
        return;
    }

    std::filesystem::path filepath = filepath_str;

    if (!std::filesystem::exists(filepath)) {
        std::cerr << "Fatal: could not open '" << filepath << "': no such file \n";
        return;
    }

    try {
        std::string content = readFile(filepath);

        std::string hash;
        if (write_flag) {
            hash = repo.writeObject(content, "blob");
        } else {
            std::string store = "blob " + std::to_string(content.size()) + '\0' + content;
            hash = generate_sha1(store);
        }

        std::cout << hash << "\n";
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }
}