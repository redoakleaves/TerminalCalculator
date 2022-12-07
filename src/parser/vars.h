#ifndef _VARS_H_
#define _VARS_H_

#include <string>
#include <map>

#include <re2/re2.h>

#include "tools/entry.h"

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
        int ParseVarDefinitions(Tools::Entry& entry, std::string& substring, int final = 0);
        /**
         * @brief Searches for variable usage within a substring.
         * 
         * @param entry Reference to the current entry.
         * @param substring Reference to the substring.
        */
        void ParseVarUsage(Tools::Entry& entry, std::string& substring);

    private:
        static const re2::RE2 varDefinitionExpression;
        static const re2::RE2 varUsageExpression;

        static const std::map<std::string, double> constStore;
        std::map<std::string, double> m_VarStore;
    };
}

#endif
