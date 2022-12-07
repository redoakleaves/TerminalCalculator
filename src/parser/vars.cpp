#include <string>
#include <sstream>
#include <map>
#define _USE_MATH_DEFINES
#include <cmath>

#include <re2/re2.h>

#include "tools/entry.h"
#include "vars.h"

namespace Parser {
    const re2::RE2 VarParser::varDefinitionExpression("^([a-zA-Z]+)=(-?\\d+(?:[\\.\\,]\\d+)?)$");
    const re2::RE2 VarParser::varUsageExpression("([a-zA-Z]+)(?:[^\\(a-zA-Z]|$)");

    const std::map<std::string, double> VarParser::constStore = {
        { "pi", M_PI },
        { "e", M_E }
    };

    int VarParser::ParseVarDefinitions(Tools::Entry& entry, std::string& substring, int final) {
        // Detect multiple equal signs
        if (substring.find('=') != substring.rfind('='))
            return 0;

        std::string varName;
        double varDefinition;
        if (re2::RE2::FullMatch(substring, varDefinitionExpression, &varName, &varDefinition)) {
            if (constStore.count(varName))
                return 0;

            if (final)
                m_VarStore[varName] = varDefinition;

            return 1;
        }

        return 0;
    }
    void VarParser::ParseVarUsage(Tools::Entry& entry, std::string& substring) {
        re2::StringPiece match;
        std::stringstream stream;
        while (re2::RE2::PartialMatch(substring, varUsageExpression, &match)) {
            std::string varName = match.ToString();
            const std::map<std::string, double>& varSource = constStore.count(varName) ? constStore : m_VarStore;
            if (varSource.count(varName)) {
                stream.clear();
                stream.str("");
                stream << varSource.at(varName);
                substring.replace(match.data() - substring.data(), varName.length(), stream.str());
            } else {
                break;
            }
        }
    }
}
