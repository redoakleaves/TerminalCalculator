#include "Vars.h"

#include <re2/re2.h>

#include <numbers>
#include <sstream>
#include <string>
#include <unordered_map>

namespace Parser {

static const re2::RE2 kVarDefinitionExpression("^([a-zA-Z]+)=(-?\\d+(?:[\\.\\,]\\d+)?)$");
static const re2::RE2 kVarUsageExpression("([a-zA-Z]+)(?:[^\\(a-zA-Z]|$)");

static const std::unordered_map<std::string, double> kConstStore = {
    { "pi", std::numbers::pi },
    { "e", std::numbers::e }
};

bool VarParser::ParseVarDefinitions(const std::string& substring, bool final) {
    // Detect multiple equal signs
    if (substring.find('=') != substring.rfind('=')) {
        return false;
    }

    std::string varName;
    double varDefinition;
    if (re2::RE2::FullMatch(substring, kVarDefinitionExpression, &varName, &varDefinition)) {
        if (kConstStore.count(varName)) {
            return false;
        }

        if (final) {
            m_VarStore[varName] = varDefinition;
        }

        return true;
    }

    return false;
}
void VarParser::ParseVarUsage(std::string& substring) const {
    re2::StringPiece match;
    std::stringstream stream;
    while (re2::RE2::PartialMatch(substring, kVarUsageExpression, &match)) {
        const std::string varName = match.ToString();
        const std::unordered_map<std::string, double>& varSource = kConstStore.count(varName) ? kConstStore : m_VarStore;
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
