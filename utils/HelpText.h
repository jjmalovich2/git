#include <map>
#include <string>

struct HelpInfo {
    std::string summary;
    std::string usage;
    std::string description;
};

namespace HelpText {
    const char* const InitSummary = "Create an empty Git repository";
    const char* const InitUsage   = "git init [directory]";
    const char* const InitFull    = R"(Creates a .git directory...)";
    
    const char* const AddSummary  = "Add file contents to the index";
    const char* const AddUsage    = "git add <pathspec>";
    const char* const AddFull     = R"(Updates the index...)";

    const char* const HelpSummary  = "Display details about a command";
    const char* const HelpUsage    = "git help <command> OR git <command> -h";
    const char* const HelpFull     = R"(Shows help about a command)";
}

inline std::map<std::string, HelpInfo> help_reg = {
    {"init", {HelpText::InitSummary, HelpText::InitUsage, HelpText::InitFull}},
    {"add", {HelpText::AddSummary, HelpText::AddUsage, HelpText::AddFull}},
    {"help", {HelpText::HelpSummary, HelpText::HelpUsage, HelpText::HelpFull}},
};