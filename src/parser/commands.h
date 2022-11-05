#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <string>

#include "tools/entry.h"

enum Commands {
    Exit = 1
};

/**
 * @brief Searches for command usage within a substring.
 * 
 * @param entry Reference to the current entry.
 * @param substring Reference to the substring.
 * 
 * @return Returns 0 if no command is found, otherwise an integer representing the command is returned.
*/
int parse_commands(Tools::Entry& entry, std::string& substring);

#endif
