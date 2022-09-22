#include <regex>

#include "parser.h"
#include "algebra.h"
#include "vars.h"

static const std::regex subexpression("\\((?=[^(])[\\w\\*\\/\\+\\-]+\\)");

int parse_substring(Entry& entry, std::string& substring) {
    parse_algebra(entry, substring);

    if (substring.find('=') != std::string::npos)
        return parse_var_def(entry, substring);
    else
        return std::regex_match(substring, std::regex("[-]?\\d+[\\.\\,]?(?:\\d+)?"));
}

void parse(Entry& entry, int final) {
    entry.set_stylized(entry.raw_content);

    // Create working copy of input and remove spaces
    std::string working_copy = entry.raw_content;
    working_copy.erase(std::remove_if(working_copy.begin(), working_copy.end(), ::isspace), working_copy.end());

    // Resolve vars
    if (working_copy.find('=') == std::string::npos)
        parse_var_usage(entry, working_copy);

    // Search for subexpressions
    std::smatch match;
    while (std::regex_search(working_copy, match, subexpression)) {
        std::string subexpression = match.str().substr(1, match.str().length() - 2);
        parse_substring(entry, subexpression);
        working_copy.replace(match.position(), match.length(), subexpression);
    }

    // Parse remaining string
    int valid = parse_substring(entry, working_copy);
    if (valid) {
        entry.set_result(working_copy);
        if (final)
            parse_var_def(entry, working_copy, 1);
    } else {
        std::string result_empty = std::string("");
        entry.set_result(result_empty);
    }
}
