#include <iostream>
#include <sstream>
#include <regex>
#include <map>

#include "func.h"

static const std::regex func_def_expression("^([a-zA-Z]+)\\(([a-zA-Z]+(?:;[a-zA-Z]+)*)\\)=(.+)");
static const std::regex func_usage_expression("([a-zA-Z]+)\\(([a-zA-Z]+|(?:-?\\d+(?:[\\.\\,]\\d+)?)(?:;(?:[a-zA-Z]+|(?:-?\\d+(?:[\\.\\,]\\d+)?)))*)\\)");
static const std::regex param_usage_expression("[a-zA-Z]+");

static std::map<std::string, function_t> func_store;

int parse_func_def (Entry& entry, std::string& substring, int final) {
    // Detect multiple equal signs
    if (substring.find('=') != substring.rfind('='))
        return 0;

    std::smatch match;
    if (std::regex_match(substring, match, func_def_expression)) {
        if (!final)
            return 1;

        // Create new function entry
        function_t function;
        function.name = match[1].str();
        std::stringstream stream(match[2].str());
        std::string param;
        while (getline(stream, param, ';')) {
            function.params.push_back(param);
        }
        function.expression = match[3].str();
        func_store[function.name] = function;

        return 1;
    }

    return 0;
}

void parse_func_usage(Entry& entry, std::string& substring) {
    std::smatch match;
    while (std::regex_search(substring, match, func_usage_expression)) {
        // Lookup function
        if (!func_store.count(match[1].str()))
            break;

        function_t function = func_store[match[1].str()];

        // Read params
        std::map<std::string, std::string> param_store;
        std::stringstream stream(match[2].str());
        std::string param;
        int count = 0;
        while (getline(stream, param, ';') && count < function.params.size()) {
            param_store[function.params[count]] = param;
            count++;
        }
        
        // Check whether enough params were passed
        if (count != function.params.size())
            break;

        // Insert params
        int delta = 0;
        std::string expression = function.expression;
        std::string expression_copy = expression;

        std::sregex_iterator var_iterator(expression_copy.begin(), expression_copy.end(), param_usage_expression);
        std::sregex_iterator end = std::sregex_iterator();
        for (std::sregex_iterator current = var_iterator; current != end; ++current) {
            std::smatch param_match = *current;
            std::string match_string = param_match.str();

            if (param_store.count(match_string)) {
                std::string param_value = param_store[match_string];
                expression.replace(param_match.position() + delta, match_string.length(), param_value);
                delta += param_value.length() - match_string.length();
            }
        }

        substring.replace(match.position(), match.str().length(), expression);
    }
}
