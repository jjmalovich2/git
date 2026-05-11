#pragma once
#include "../utils/Command.h"

class InitCommand : public Command {
public:
    void execute(const std::vector<std::string>& args, Repository& repo) override;
};