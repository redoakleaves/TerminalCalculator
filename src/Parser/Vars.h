#pragma once

#include <string>
#include <unordered_map>

namespace Parser {

class VarParser {
public:
    /**
     * @brief Searches for variable definition in substring. A valid definition is only stored during the final pass.
     *
     * @param entry Reference to the current entry.
     * @param substring Reference to the substring.
     * @param final Boolean signaling final pass.
     *
     * @return 1 on success, 0 otherwise.
    */
    bool ParseVarDefinitions(const std::string& substring, bool final = false);
    /**
     * @brief Searches for variable usage within a substring.
     *
     * @param entry Reference to the current entry.
     * @param substring Reference to the substring.
    */
    void ParseVarUsage(std::string& substring) const;

private:
    std::unordered_map<std::string, double> m_VarStore;
};

}
