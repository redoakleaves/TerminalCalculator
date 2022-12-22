#ifndef _PARSER_H_
#define _PARSER_H_

#include <re2/re2.h>

#include "tools/entry.h"
#include "commands.h"
#include "arithmetic.h"
#include "func.h"
#include "vars.h"

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
        int Parse(Tools::Entry& entry, int final = 0);

    private:
        static const re2::RE2 subExpression;
        static const re2::RE2 validResultExpression;

        ArithmeticParser m_ArithmeticParser;
        CommandParser m_CommandParser;
        FuncParser m_FuncParser;
        VarParser m_VarParser;
    };
}

#endif
