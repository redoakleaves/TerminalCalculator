#pragma once

#include <string>

namespace Parser {

class ArithmeticParser {
public:
    /**
     * @brief Parses arithmetic expressions within a substring.
     *
     * @param substring Reference to the substring.
    */
    static void ParseArithmetic(std::string& substring);
};

}
