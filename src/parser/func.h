#ifndef _FUNC_H_
#define _FUNC_H_

#include <string>
#include <map>
#include <vector>

#include <re2/re2.h>

#include "tools/entry.h"

namespace Parser {
    typedef struct {
        std::string name;
        std::vector<std::string> params;
        std::string expression;
    } function_t;

    class FuncParser {
    public:
        /**
         * @brief Parses function defintion in substring. A valid definition is only stored during the final pass.
         * 
         * @param entry Reference to the current entry.
         * @param substring Reference to the substring.
         * @param final Boolean signaling final pass.
         * 
         * @return 1 on success, 0 otherwise.
        */
        int ParseFuncDefinition(Tools::Entry& entry, std::string& substring, int final = 0);
        /**
         * @brief Searches for and resolves usage of user-defined functions within a substring.
         * 
         * @param entry Reference to the current entry.
         * @param substring Reference to the substring.
        */
        void ParseFuncUsage(Tools::Entry& entry, std::string& substring);
        /**
         * @brief Searches for and resolves usage of predefined functions within a substring.
         * 
         * @param entry Reference to the current entry.
         * @param substring Reference to the substring.
        */
        void ParseConstFuncUsage(Tools::Entry& entry, std::string& substring);

    private:
        static const re2::RE2 funcDefinitionExpression;
        static const re2::RE2 funcUsageExpression;
        static const re2::RE2 paramUsageExpression;
        static const re2::RE2 constFuncUsageExpression;

        std::map<std::string, function_t> m_FuncStore;
        static const std::map<std::string, int> constFuncStore;
        static const std::vector<std::string> trigonometricFuncList;
    };
}

#endif
