#include <string>
#include <sstream>
#include <map>
#define _USE_MATH_DEFINES
#include <cmath>

#include <re2/re2.h>

#include "tools/entry.h"
#include "vars.h"

static const re2::RE2 var_def_expression("^([a-zA-Z]+)=(-?\\d+(?:[\\.\\,]\\d+)?)$");
static const re2::RE2 var_usage_expression("([a-zA-Z]+)(?:[^\\(a-zA-Z]|$)");

static std::map<std::string, double> var_store;
static std::map<std::string, double> const_store = {
    { "pi", M_PI },
    { "e", M_E }
};

int parse_var_def(Tools::Entry& entry, std::string& substring, int final) {
    // Detect multiple equal signs
    if (substring.find('=') != substring.rfind('='))
        return 0;

    std::string var_name;
    double var_definition;
    if (re2::RE2::FullMatch(substring, var_def_expression, &var_name, &var_definition)) {
        if (const_store.count(var_name))
            return 0;

        if (final)
            var_store[var_name] = var_definition;

        return 1;
    }

    return 0;
}

void parse_var_usage(Tools::Entry& entry, std::string& substring) {
    re2::StringPiece match;
    std::stringstream stream;
    while (re2::RE2::PartialMatch(substring, var_usage_expression, &match)) {
        std::string var_name = match.ToString();
        std::map<std::string, double>& var_source = const_store.count(var_name) ? const_store : var_store;
        if (var_source.count(var_name)) {
            stream.clear();
            stream.str("");
            stream << var_source.at(var_name);
            substring.replace(match.data() - substring.data(), var_name.length(), stream.str());
        } else {
            break;
        }
    }
}
