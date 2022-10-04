#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "entry.h"

enum Commands {
    Exit = 1
};

int parse_commands(Entry& entry, std::string& substring);

#endif
