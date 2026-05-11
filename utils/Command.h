#pragma once
#include "Repository.h"
#include <vector>
#include <string>

class Command {
public:
    virtual ~Command() = default;

    virtual void execute(const std::vector<std::string>& args, Repository& repo) = 0;
};