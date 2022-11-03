#include <sstream>
#include <string>
#include <algorithm>

#include <re2/re2.h>

#include "tools/color.h"
#include "commands.h"

static const re2::RE2 command_expression("^:([a-zA-Z]+)");

int parse_commands(Entry& entry, std::string& substring) {
    re2::StringPiece match;
    if (re2::RE2::FullMatch(substring, command_expression, &match)) {
        // Convert command to lowercase
        std::string command = match.ToString();
        std::transform(command.begin(), command.end(), command.begin(), [](unsigned char c) {
            return std::tolower(c);
        });

        // Colorize command in entry
        std::stringstream stream;
        stream << '{' << COLOR_COMMAND << '}';
        stream << ':' << match.ToString();
        std::string stylized = stream.str();
        entry.set_stylized(stylized);

        // Return int representing command
        if (command == "exit")
            return Commands::Exit;
    }

    return 0;
}
