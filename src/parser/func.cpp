#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>

#include <re2/re2.h>

#include "global.h"
#include "tools/entry.h"
#include "func.h"

namespace Parser {
    const re2::RE2 FuncParser::funcDefinitionExpression("^([a-zA-Z]+)\\(([a-zA-Z]+(?:;[a-zA-Z]+)*)\\)=(.+)$");
    const re2::RE2 FuncParser::funcUsageExpression("([a-zA-Z]+)\\(((?:[a-zA-Z]+|(?:-?\\d+(?:[\\.\\,]\\d+)?))(?:;(?:[a-zA-Z]+|(?:-?\\d+(?:[\\.\\,]\\d+)?)))*)\\)");
    const re2::RE2 FuncParser::paramUsageExpression("([a-zA-Z]+)(?:[^\\(a-zA-Z]|$)");
    const re2::RE2 FuncParser::constFuncUsageExpression("([a-zA-Z]+)\\(((?:-?\\d+(?:[\\.\\,]\\d+)?)(?:;(?:-?\\d+(?:[\\.\\,]\\d+)?))*)\\)");

    const std::map<std::string, int> FuncParser::constFuncStore = {
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
    const std::vector<std::string> FuncParser::trigonometricFuncList = { "sin", "cos", "tan", "csc", "sec", "cot" };

    int FuncParser::ParseFuncDefinition(Tools::Entry& entry, std::string& substring, int final) {
        // Detect multiple equal signs
        if (substring.find('=') != substring.rfind('='))
            return 0;

        std::string funcName;
        std::string funcParams;
        std::string funcExpression;
        if (re2::RE2::FullMatch(substring, funcDefinitionExpression, &funcName, &funcParams, &funcExpression)) {
            if (constFuncStore.count(funcName))
                return 0;

            if (!final)
                return 1;

            // Create new function entry
            function_t function;
            function.name = funcName;
            std::stringstream stream(funcParams);
            std::string param;
            while (getline(stream, param, ';')) {
                function.params.push_back(param);
            }
            function.expression = funcExpression;
            m_FuncStore[function.name] = function;

            return 1;
        }

        return 0;
    }

    void FuncParser::ParseFuncUsage(Tools::Entry& entry, std::string& substring) {
        re2::StringPiece funcName;
        std::string funcParams;
        while (re2::RE2::PartialMatch(substring, funcUsageExpression, &funcName, &funcParams)) {
            function_t* function = nullptr;

            // Lookup function
            if (m_FuncStore.count(funcName.ToString())) {
                function = &m_FuncStore[funcName.ToString()];
            } else {
                break;
            }

            // Read params
            std::map<std::string, std::string> paramStore;
            std::stringstream stream(funcParams);
            std::string param;
            int count = 0;
            while (getline(stream, param, ';') && count < function->params.size()) {
                paramStore[function->params[count]] = param;
                count++;
            }
            
            // Check whether enough params were passed
            if (count != function->params.size())
                break;

            // Insert params
            std::string expression = function->expression;
            std::string expressionCopy = expression;

            int offset = 0;
            int delta = 0;
            re2::StringPiece paramMatch;
            while (re2::RE2::PartialMatch(expressionCopy.data() + offset, paramUsageExpression, &paramMatch) && offset < expressionCopy.length()) {
                offset = paramMatch.data() - expressionCopy.data();
                std::string matchString = paramMatch.ToString();
                if (paramStore.count(matchString)) {
                    std::string paramValue = paramStore[matchString];
                    expression.replace(offset + delta, matchString.length(), paramValue);
                    delta += paramValue.length() - matchString.length();
                }
                offset++;
            }

            substring.replace(funcName.data() - substring.data(), funcName.length() + funcParams.length() + 2, "(" + expression + ")");
        }
    }

    void FuncParser::ParseConstFuncUsage(Tools::Entry& entry, std::string& substring) {
        re2::StringPiece funcName;
        std::string funcParams;
        std::stringstream stream;
        while (re2::RE2::PartialMatch(substring, constFuncUsageExpression, &funcName, &funcParams))
        {
            std::string funcNameString = funcName.ToString();

            // Check whether function exists
            if (!constFuncStore.count(funcNameString))
                break;

            // Read params
            std::vector<double> paramList;
            std::stringstream stream(funcParams);
            std::string param;
            int count = 0;
            while (getline(stream, param, ';') && count < constFuncStore.at(funcNameString)) {
                paramList.push_back(std::stod(param));
                count++;
            }

            // Check whether enough params were passed
            if (count != constFuncStore.at(funcNameString))
                break;

            // Convert first param to radians if necessary
            if (std::find(trigonometricFuncList.begin(), trigonometricFuncList.end(), funcNameString) != trigonometricFuncList.end()) {
                if (globalstate.m_UseDeg)
                    paramList[0] *= M_PI / 180.0;
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
