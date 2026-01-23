#include "Func.h"
#include "global.h"

#include <re2/re2.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <map>
#include <numbers>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace Parser {

static const re2::RE2 kFuncDefinitionExpression("^([a-zA-Z]+)\\(([a-zA-Z]+(?:;[a-zA-Z]+)*)\\)=(.+)$");
static const re2::RE2 kFuncUsageExpression("([a-zA-Z]+)\\(((?:[a-zA-Z]+|(?:-?\\d+(?:[\\.\\,]\\d+)?))(?:;(?:[a-zA-Z]+|(?:-?\\d+(?:[\\.\\,]\\d+)?)))*)\\)");
static const re2::RE2 kParamUsageExpression("([a-zA-Z]+)(?:[^\\(a-zA-Z]|$)");
static const re2::RE2 kConstFuncUsageExpression("([a-zA-Z]+)\\(((?:-?\\d+(?:[\\.\\,]\\d+)?)(?:;(?:-?\\d+(?:[\\.\\,]\\d+)?))*)\\)");

static const std::unordered_map<std::string, int> kConstFuncStore = {
    { "pow", 2 },
    { "sqrt", 1 },
    { "ln", 1 },
    { "log", 1 },

    { "abs", 1 },
    { "ceil", 1 },
    { "floor", 1 },

    // Trigonometry
    { "sin", 1 },
    { "cos", 1 },
    { "tan", 1 },
    { "csc", 1 },
    { "sec", 1 },
    { "cot", 1 }
};
static const std::array<std::string, 6> kTrigonometricFuncList = { "sin", "cos", "tan", "csc", "sec", "cot" };

bool FuncParser::ParseFuncDefinition(const std::string& substring, bool final) {
    // Detect multiple equal signs
    if (substring.find('=') != substring.rfind('=')) {
        return false;
    }

    std::string funcName;
    std::string funcParams;
    std::string funcExpression;
    if (re2::RE2::FullMatch(substring, kFuncDefinitionExpression, &funcName, &funcParams, &funcExpression)) {
        if (kConstFuncStore.count(funcName)) {
            return false;
        }

        if (!final) {
            return true;
        }

        // Create new function entry
        Function_t function;
        function.name = funcName;
        std::stringstream stream(funcParams);
        std::string param;
        while (getline(stream, param, ';')) {
            function.params.push_back(param);
        }
        function.expression = funcExpression;
        funcStore_m[function.name] = function;

        return true;
    }

    return false;
}

void FuncParser::ParseFuncUsage(std::string& substring) const {
    re2::StringPiece funcName;
    std::string funcParams;
    while (re2::RE2::PartialMatch(substring, kFuncUsageExpression, &funcName, &funcParams)) {
        const Function_t* function = nullptr;

        // Lookup function
        if (funcStore_m.count(funcName.ToString())) {
            function = &funcStore_m.at(funcName.ToString());
        } else {
            break;
        }

        // Read params
        std::map<std::string, std::string> paramStore;
        std::stringstream stream(funcParams);
        std::string param;
        size_t count = 0;
        while (getline(stream, param, ';') && count < function->params.size()) {
            paramStore[function->params[count]] = param;
            count++;
        }

        // Check whether enough params were passed
        if (count != function->params.size()) {
            break;
        }

        // Insert params
        std::string expression = function->expression;
        const std::string expressionCopy = expression;

        size_t offset = 0;
        int delta = 0;
        re2::StringPiece paramMatch;
        while (re2::RE2::PartialMatch(expressionCopy.data() + offset, kParamUsageExpression, &paramMatch) && offset < expressionCopy.length()) {
            offset = paramMatch.data() - expressionCopy.data();
            const std::string matchString = paramMatch.ToString();
            if (paramStore.count(matchString)) {
                const std::string paramValue = paramStore[matchString];
                expression.replace(offset + delta, matchString.length(), paramValue);
                delta += paramValue.length() - matchString.length();
            }
            offset++;
        }

        substring.replace(funcName.data() - substring.data(), funcName.length() + funcParams.length() + 2, "(" + expression + ")");
    }
}

void FuncParser::ParseConstFuncUsage(std::string& substring) const {
    re2::StringPiece funcName;
    std::string funcParams;
    std::stringstream stream;
    while (re2::RE2::PartialMatch(substring, kConstFuncUsageExpression, &funcName, &funcParams)) {
        const std::string funcNameString = funcName.ToString();

        // Check whether function exists
        if (!kConstFuncStore.count(funcNameString)) {
            break;
        }

        // Read params
        std::vector<double> paramList;
        std::stringstream stream(funcParams);
        std::string param;
        int count = 0;
        while (getline(stream, param, ';') && count < kConstFuncStore.at(funcNameString)) {
            paramList.push_back(std::stod(param));
            count++;
        }

        // Check whether enough params were passed
        if (count != kConstFuncStore.at(funcNameString)) {
            break;
        }

        // Convert first param to radians if necessary
        if (std::find(kTrigonometricFuncList.begin(), kTrigonometricFuncList.end(), funcNameString) != kTrigonometricFuncList.end()) {
            if (globalState.useDeg_m) {
                paramList[0] *= std::numbers::pi / 180.0;
            }
        }

        // Calculate result
        double result = 0;
        if (funcNameString == "pow") {
            result = std::pow(paramList[0], paramList[1]);
        } else if (funcNameString == "sqrt") {
            result = std::sqrt(paramList[0]);
        } else if (funcNameString == "ln") {
            result = std::log(paramList[0]);
        } else if (funcNameString == "log") {
            result = std::log10(paramList[0]);
        }

        else if (funcNameString == "abs") {
            result = std::abs(paramList[0]);
        } else if (funcNameString == "ceil") {
            result = std::ceil(paramList[0]);
        } else if (funcNameString == "floor") {
            result = std::floor(paramList[0]);
        }
        // Trigonometry
        else if (funcNameString == "sin") {
            result = std::sin(paramList[0]);
        } else if (funcNameString == "cos") {
            result = std::cos(paramList[0]);
        } else if (funcNameString == "tan") {
            result = std::tan(paramList[0]);
        } else if (funcNameString == "csc") {
            result = 1.0 / std::sin(paramList[0]);
        } else if (funcNameString == "sec") {
            result = 1.0 / std::cos(paramList[0]);
        } else if (funcNameString == "cot") {
            result = 1.0 / std::tan(paramList[0]);
        }

        stream.clear();
        stream.str("");
        stream << result;
        substring.replace(funcName.data() - substring.data(), funcName.length() + funcParams.length() + 2, stream.str());
    }
}

}
