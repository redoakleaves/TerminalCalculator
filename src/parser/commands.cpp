#include <sstream>
#include <regex>
#include <algorithm>

#include "tools/color.h"
#include "commands.h"

static const std::regex command_expression("^:([a-zA-Z]+)");

int parse_commands(Entry& entry, std::string& substring) {
    std::smatch match;
    if (std::regex_match(substring, match, command_expression)) {
        // Convert command to lowercase
        std::string command = match[1].str();
        std::transform(command.begin(), command.end(), command.begin(), [](unsigned char c) {
            return std::tolower(c);
        });

        // Colorize command in entry
        std::stringstream stream;
        stream << '{' << COLOR_COMMAND << '}';
        stream << match[0].str();
        std::string stylized = stream.str();
        entry.set_stylized(stylized);

        // Return int representing command
        if (command.compare("exit") == 0)
            return Commands::Exit;
    }

    return 0;
}
