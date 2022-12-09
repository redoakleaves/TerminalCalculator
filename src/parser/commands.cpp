#include <sstream>
#include <string>
#include <algorithm>

#include <re2/re2.h>

#include "global.h"
#include "tools/color.h"
#include "tools/entry.h"
#include "commands.h"

namespace Parser {
    const re2::RE2 CommandParser::commandExpression("^:([a-zA-Z]+)$");

    int CommandParser::ParseSubstring(Tools::Entry& entry, std::string& substring, int final) {
        int commandValue = 0;
        std::string commandString;
        if (re2::RE2::FullMatch(substring, commandExpression, &commandString)) {
            std::string commandCopy = commandString;

            // Convert command to lowercase
            std::transform(commandString.begin(), commandString.end(), commandString.begin(), [](unsigned char c) {
                return std::tolower(c);
            });

            // Check command validity
            if (commandString == "exit") {
                if (final)
                    commandValue = Commands::Exit;
                else
                    commandValue = Commands::NoAction;
            }
            // Degree/Radian switching
            else if (commandString == "deg") {
                if (final)
                    globalstate.m_UseDeg = 1;
                commandValue = Commands::NoAction;
            } else if (commandString == "rad") {
                if (final)
                    globalstate.m_UseDeg = 0;
                commandValue = Commands::NoAction;
            // Invalid command
            } else {
                return 0;
            }

            // Colorize command in entry
            std::stringstream stream;
            stream << '{' << COLOR_COMMAND << '}';
            stream << ':' << commandCopy;
            std::string stylizedString = stream.str();
            entry.SetStylized(stylizedString);
        }

        return commandValue;
    }
}
