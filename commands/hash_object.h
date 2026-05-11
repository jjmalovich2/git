#pragma once
#include "../utils/Command.h"
#include "../utils/Repository.h"

class HashObjectCommand : public Command {
public:
    void execute(const std::vector<std::string>& args, Repository& repo) override;
};