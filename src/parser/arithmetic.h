#ifndef _ARITHMETIC_H_
#define _ARITHMETIC_H_

#include <string>

#include <re2/re2.h>

#include "tools/entry.h"

namespace Parser {
    class ArithmeticParser {
    public:
        /**
         * @brief Parses arithmetic expressions within a substring.
         * 
         * @param entry Reference to the current entry.
         * @param substring Reference to the substring.
        */
        void ParseArithmetic(Tools::Entry& entry, std::string& substring);

    private:
        static const re2::RE2 arithmeticExpression;
        static const re2::RE2 doubleSignExpression;

        int IsOperand(char c);
        void ReplaceDoubleSigns(std::string& substring);
    };
}

#endif
