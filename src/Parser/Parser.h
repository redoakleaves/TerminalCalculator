#pragma once

#include "Func.h"
#include "Vars.h"

// Forward declarations
namespace Tools {
    class Entry;
}

namespace Parser {

class Parser {
public:
    /**
     * @brief Parses the input string stored in the entry passed along.
     *
     * @param entry Reference to the entry.
     * @param final Boolean signaling final pass.
     *
     * @return -1 on error, 0 on success and >0 if command is detected.
    */
    int Parse(Tools::Entry& entry, bool final = false);

private:
    FuncParser funcParser_m;
    VarParser varParser_m;
};

}
