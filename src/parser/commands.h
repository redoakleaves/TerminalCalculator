#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <string>

#include "tools/entry.h"

enum Commands {
    NoAction = 1,
    Exit = 2
};

/**
 * @brief Searches for command usage within a substring.
 * 
 * @param entry Reference to the current entry.
 * @param substring Reference to the substring.
 * @param final Boolean signaling final pass.
 * 
 * @return Returns 0 if no command is found, otherwise an integer representing the command is returned.
*/
int parse_commands(Tools::Entry& entry, std::string& substring, int final = 0);

#endif
