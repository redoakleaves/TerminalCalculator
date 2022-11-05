#include <sstream>
#include <string>
#include <algorithm>

#include <re2/re2.h>

#include "global.h"
#include "tools/color.h"
#include "tools/entry.h"
#include "commands.h"

static const re2::RE2 command_expression("^:([a-zA-Z]+)$");

int parse_commands(Tools::Entry& entry, std::string& substring, int final) {
    int command_value = 0;
    std::string command;
    if (re2::RE2::FullMatch(substring, command_expression, &command)) {
        std::string command_copy = command;

        // Convert command to lowercase
        std::transform(command.begin(), command.end(), command.begin(), [](unsigned char c) {
            return std::tolower(c);
        });

        // Check command validity
        if (command == "exit") {
            if (final)
                command_value = Commands::Exit;
            else
                command_value = Commands::NoAction;
        }
        // Degree/Radian switching
        else if (command == "deg") {
            if (final)
                globalstate.use_deg = 1;
            command_value = Commands::NoAction;
        } else if (command == "rad") {
            if (final)
                globalstate.use_deg = 0;
            command_value = Commands::NoAction;
        } else {
            return 0;
        }

        // Colorize command in entry
        std::stringstream stream;
        stream << '{' << COLOR_COMMAND << '}';
        stream << ':' << command_copy;
        std::string stylized = stream.str();
        entry.set_stylized(stylized);
    }

    return command_value;
}
