#include <sstream>
#include <string>
#include <algorithm>

#include <re2/re2.h>

#include "global.h"
#include "tools/color.h"
#include "tools/entry.h"
#include "commands.h"

static const re2::RE2 command_expression("^:([a-zA-Z]+)$");

int parse_commands(Tools::Entry& entry, std::string& substring) {
    std::string command;
    if (re2::RE2::FullMatch(substring, command_expression, &command)) {
        std::string command_copy = command;

        // Convert command to lowercase
        std::transform(command.begin(), command.end(), command.begin(), [](unsigned char c) {
            return std::tolower(c);
        });

        // Colorize command in entry
        std::stringstream stream;
        stream << '{' << COLOR_COMMAND << '}';
        stream << ':' << command_copy;
        std::string stylized = stream.str();
        entry.set_stylized(stylized);

        // Return int representing command
        if (command == "exit")
            return Commands::Exit;

        else if (command == "deg") {
            globalstate.use_deg = 1;
        } else if (command == "rad") {
            globalstate.use_deg = 0;
        }
    }

    return 0;
}
