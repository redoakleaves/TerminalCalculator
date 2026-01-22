#pragma once

#include <optional>
#include <string>

// Forward declarations
namespace Tools {
    class Entry;
}

namespace Parser {

enum Command {
    NoAction = 1,
    Exit = 2
};

class CommandParser {
public:
    /**
     * @brief Searches for command usage within a substring.
     *
     * @param entry Reference to the current entry.
     * @param substring Reference to the substring.
     * @param final Boolean signaling final pass.
     *
     * @return Returns optional containing the command enum if found.
    */
    static std::optional<Command> ParseSubstring(Tools::Entry& entry, const std::string& substring, bool final = false);
};

}
