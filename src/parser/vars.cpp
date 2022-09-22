#include <sstream>
#include <regex>
#define _USE_MATH_DEFINES
#include <math.h>
#include <map>

#include "vars.h"

static const std::regex var_def_expression("^([a-zA-Z]+)=(-?\\d+(?:[\\.\\,]\\d+)?)$");
static const std::regex var_usage_expression("[a-zA-Z]+");

static std::map<std::string, double> var_store;
static std::map<std::string, double> const_store = {
    { "pi", M_PI },
    { "e", M_E }
};

int parse_var_def(Entry& entry, std::string& substring, int final) {
    std::smatch match;
    if (std::regex_match(substring, match, var_def_expression) && !const_store.count(match[1].str())) {
        if (!final)
            return 1;

        std::stringstream stream;
        stream.str(match[2].str());

        double value;
        stream >> value;

        var_store.insert(std::pair<std::string, double>(match[1].str(), value));

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
