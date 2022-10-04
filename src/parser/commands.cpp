#include <regex>

#include "tools/color.h"
#include "commands.h"

static const std::regex command_expression("^:([a-zA-Z]+)");

int parse_commands(Entry& entry, std::string& substring) {
    std::smatch match;
    if (std::regex_match(substring, match, command_expression)) {
        std::string command = match[1].str();
        std::transform(command.begin(), command.end(), command.begin(), [](unsigned char c) {
            return std::tolower(c);
        });

        std::stringstream stream;
        stream << '{';
        stream << COLOR_COMMAND;
        stream << '}';
        stream << match[0].str();
        std::string stylized = stream.str();
        entry.set_stylized(stylized);

        if (command.compare("exit") == 0)
            return Commands::Exit;
    }

    return 0;
}
