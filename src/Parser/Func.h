#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace Parser {

typedef struct {
    std::string name;
    std::vector<std::string> params;
    std::string expression;
} Function_t;

class FuncParser {
public:
    /**
     * @brief Parses function defintion in substring. A valid definition is only stored during the final pass.
     *
     * @param substring Reference to the substring.
     * @param final Boolean signaling final pass.
     *
     * @return 1 on success, 0 otherwise.
    */
    [[nodiscard]] bool ParseFuncDefinition(const std::string& substring, bool final = false);
    /**
     * @brief Searches for and resolves usage of user-defined functions within a substring.
     *
     * @param substring Reference to the substring.
    */
    void ParseFuncUsage(std::string& substring) const;
    /**
     * @brief Searches for and resolves usage of predefined functions within a substring.
     *
     * @param substring Reference to the substring.
    */
    void ParseConstFuncUsage(std::string& substring) const;

private:
    std::unordered_map<std::string, Function_t> funcStore_m;
};

}
