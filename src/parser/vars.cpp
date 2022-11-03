#include <sstream>
#include <string>
#include <regex>
#define _USE_MATH_DEFINES
#include <math.h>
#include <map>

#include <re2/re2.h>

#include "vars.h"

static const re2::RE2 var_def_expression("^([a-zA-Z]+)=(-?\\d+(?:[\\.\\,]\\d+)?)$");
static const std::regex var_usage_expression("([a-zA-Z]+)(?=[^\\(a-zA-Z]|$)");

static std::map<std::string, double> var_store;
static std::map<std::string, double> const_store = {
    { "pi", M_PI },
    { "e", M_E }
};

int parse_var_def(Entry& entry, std::string& substring, int final) {
    // Detect multiple equal signs
    if (substring.find('=') != substring.rfind('='))
        return 0;

    std::string var_name;
    double var_definition;
    if (re2::RE2::FullMatch(substring, var_def_expression, &var_name, &var_definition)) {
        if (!final)
            return 1;

        var_store[var_name] = var_definition;

        return 1;
    }

    return 0;
}

void parse_var_usage(Entry& entry, std::string& substring) {
    int delta = 0;
    std::string substring_copy = substring;

    std::sregex_iterator var_iterator(substring_copy.begin(), substring_copy.end(), var_usage_expression);
    std::sregex_iterator end = std::sregex_iterator();
    for (std::sregex_iterator current = var_iterator; current != end; ++current) {
        std::smatch match = *current;
        std::string match_string = match.str();

        std::map<std::string, double>& var_source = const_store.count(match_string) ? const_store : var_store;
        if (var_source.count(match_string)) {
            std::stringstream stream;
            stream << var_source.at(match_string);
            substring.replace(match.position() + delta, match_string.length(), stream.str());
            delta += stream.str().length() - match_string.length();
        }
    }
}
