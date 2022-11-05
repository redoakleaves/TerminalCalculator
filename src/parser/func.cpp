#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <cmath>

#include <re2/re2.h>

#include "func.h"

static const re2::RE2 func_def_expression("^([a-zA-Z]+)\\(([a-zA-Z]+(?:;[a-zA-Z]+)*)\\)=(.+)$");
static const re2::RE2 func_usage_expression("([a-zA-Z]+)\\(((?:[a-zA-Z]+|(?:-?\\d+(?:[\\.\\,]\\d+)?))(?:;(?:[a-zA-Z]+|(?:-?\\d+(?:[\\.\\,]\\d+)?)))*)\\)");
static const re2::RE2 param_usage_expression("([a-zA-Z]+)(?:[^\\(a-zA-Z]|$)");

static const re2::RE2 const_func_usage_expression("([a-zA-Z]+)\\(((?:-?\\d+(?:[\\.\\,]\\d+)?)(?:;(?:-?\\d+(?:[\\.\\,]\\d+)?))*)\\)");

static std::map<std::string, function_t> func_store;
static const std::map<std::string, int> const_func_store = {
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

int parse_func_def(Entry& entry, std::string& substring, int final) {
    // Detect multiple equal signs
    if (substring.find('=') != substring.rfind('='))
        return 0;

    std::string func_name;
    std::string func_params;
    std::string func_expression;
    if (re2::RE2::FullMatch(substring, func_def_expression, &func_name, &func_params, &func_expression)) {
        if (!final)
            return 1;

        if (const_func_store.count(func_name))
            return 0;

        // Create new function entry
        function_t function;
        function.name = func_name;
        std::stringstream stream(func_params);
        std::string param;
        while (getline(stream, param, ';')) {
            function.params.push_back(param);
        }
        function.expression = func_expression;
        func_store[function.name] = function;

        return 1;
    }

    return 0;
}

void parse_func_usage(Entry& entry, std::string& substring) {
    re2::StringPiece func_name;
    std::string func_params;
    while (re2::RE2::PartialMatch(substring, func_usage_expression, &func_name, &func_params)) {
        function_t* function = nullptr;

        // Lookup function
        if (func_store.count(func_name.ToString())) {
            function = &func_store[func_name.ToString()];
        } else {
            break;
        }

        // Read params
        std::map<std::string, std::string> param_store;
        std::stringstream stream(func_params);
        std::string param;
        int count = 0;
        while (getline(stream, param, ';') && count < function->params.size()) {
            param_store[function->params[count]] = param;
            count++;
        }
        
        // Check whether enough params were passed
        if (count != function->params.size())
            break;

        // Insert params
        std::string expression = function->expression;
        std::string expression_copy = expression;

        int offset = 0;
        int delta = 0;
        re2::StringPiece param_match;
        while (re2::RE2::PartialMatch(expression_copy.data() + offset, param_usage_expression, &param_match) && offset < expression_copy.length()) {
            offset = param_match.data() - expression_copy.data();
            std::string match_string = param_match.ToString();
            if (param_store.count(match_string)) {
                std::string param_value = param_store[match_string];
                expression.replace(offset + delta, match_string.length(), param_value);
                delta += param_value.length() - match_string.length();
            }
            offset++;
        }

        substring.replace(func_name.data() - substring.data(), func_name.length() + func_params.length() + 2, "(" + expression + ")");
    }
}

void parse_const_func_usage(Entry& entry, std::string& substring) {
    re2::StringPiece func_name;
    std::string func_params;
    while (re2::RE2::PartialMatch(substring, const_func_usage_expression, &func_name, &func_params))
    {
        std::string func_name_string = func_name.ToString();

        // Check whether function exists
        if (!const_func_store.count(func_name_string))
            break;

        // Read params
        std::vector<double> param_list;
        std::stringstream stream(func_params);
        std::string param;
        int count = 0;
        while (getline(stream, param, ';') && count < const_func_store.at(func_name_string)) {
            param_list.push_back(std::stod(param));
            count++;
        }

        // Check whether enough params were passed
        if (count != const_func_store.at(func_name_string))
            break;

        // Calculate result
        double result = 0;
        if (func_name_string == "sqrt") {
            result = std::sqrt(param_list[0]);
        } else if (func_name_string == "ln") {
            result = std::log(param_list[0]);
        } else if (func_name_string == "log") {
            result = std::log10(param_list[0]);
        }

        else if (func_name_string == "abs") {
            result = std::abs(param_list[0]);
        } else if (func_name_string == "ceil") {
            result = std::ceil(param_list[0]);
        } else if (func_name_string == "floor") {
            result = std::floor(param_list[0]);
        }
        // Trigonometry
        else if (func_name_string == "sin") {
            result = std::sin(param_list[0]);
        } else if (func_name_string == "cos") {
            result = std::cos(param_list[0]);
        } else if (func_name_string == "tan") {
            result = std::tan(param_list[0]);
        } else if (func_name_string == "csc") {
            result = 1.0 / std::sin(param_list[0]);
        } else if (func_name_string == "sec") {
            result = 1.0 / std::cos(param_list[0]);
        } else if (func_name_string == "cot") {
            result = 1.0 / std::tan(param_list[0]);
        }

        substring.replace(func_name.data() - substring.data(), func_name.length() + func_params.length() + 2, std::to_string(result));
    }
}
