#include "Commands.h"
#include "global.h"
#include "tools/color.h"
#include "tools/entry.h"

#include <re2/re2.h>

#include <algorithm>
#include <optional>
#include <sstream>
#include <string>

namespace Parser {

static const re2::RE2 kCommandExpression("^:([a-zA-Z]+)$");

std::optional<Command> CommandParser::ParseSubstring(Tools::Entry& entry, const std::string& substring, bool final) {
    std::optional<Command> commandValue = std::nullopt;
    std::string commandString;
    if (re2::RE2::FullMatch(substring, kCommandExpression, &commandString)) {
        const std::string commandCopy = commandString;

        // Convert command to lowercase
        std::transform(commandString.begin(), commandString.end(), commandString.begin(), [](unsigned char c) {
            return std::tolower(c);
        });

        // Check command validity
        if (commandString == "exit") {
            if (final) {
                commandValue = Command::Exit;
            } else {
                commandValue = Command::NoAction;
            }
        }
        // Degree/Radian switching
        else if (commandString == "deg") {
            if (final) {
                globalstate.m_UseDeg = 1;
            }
            commandValue = Command::NoAction;
        } else if (commandString == "rad") {
            if (final) {
                globalstate.m_UseDeg = 0;
            }
            commandValue = Command::NoAction;
        // Invalid command
        } else {
            return std::nullopt;
        }

        // Colorize command in entry
        std::stringstream stream;
        stream << '{' << COLOR_COMMAND << '}';
        stream << ':' << commandCopy;
        std::string stylized = stream.str();
        entry.SetStylized(stylized);
    }

    return commandValue;
}

}
