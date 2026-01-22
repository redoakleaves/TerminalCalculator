#pragma once

#include <string>

namespace Parser {

class ArithmeticParser {
public:
    /**
     * @brief Parses arithmetic expressions within a substring.
     *
     * @param entry Reference to the current entry.
     * @param substring Reference to the substring.
    */
    static void ParseArithmetic(std::string& substring);
};

}
