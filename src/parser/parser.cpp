#include <string>
#include <algorithm>

#include <re2/re2.h>

#include "tools/entry.h"
#include "parser.h"
#include "commands.h"
#include "algebra.h"
#include "func.h"
#include "vars.h"

static const re2::RE2 sub_expression("(?:^|[^a-zA-Z\\d.,])\\(([a-zA-Z\\d.,\\^\\*\\/+-]+)\\)");
static const re2::RE2 valid_result_expression("^-?\\d+(?:[\\.\\,]\\d+)?$");

int parse(Tools::Entry& entry, int final) {
    entry.set_stylized(entry.raw_content);

    // Create working copy of input and remove spaces
    std::string working_copy = entry.raw_content;
    working_copy.erase(std::remove_if(working_copy.begin(), working_copy.end(), ::isspace), working_copy.end());

    // Check for commands
    int command = parse_commands(entry, working_copy);
    if (command)
        return command;

    // Resolve function and variable definitions
    if (working_copy.find('=') != std::string::npos) {
        if (!parse_func_def(entry, working_copy, final)) {
            parse_var_def(entry, working_copy, final);
        }
        return 0;
    }

    // Resolve function and variable usage
    parse_func_usage(entry, working_copy);
    parse_var_usage(entry, working_copy);

    // Parse pre-defined functions, subexpressions, and algebra until nothing changes
    int old_length = 0;
    do {
        old_length = working_copy.length();
        
        parse_const_func_usage(entry, working_copy);

        // Search for subexpressions
        re2::StringPiece subexpression;
        while (re2::RE2::PartialMatch(working_copy, sub_expression, &subexpression))
        {
            std::string subexpression_string = subexpression.ToString();
            parse_algebra(entry, subexpression_string);
            working_copy.replace(subexpression.data() - working_copy.data() - 1, subexpression.length() + 2, subexpression_string);
        }

        parse_algebra(entry, working_copy);
    } while (old_length != working_copy.length());

    // Check remaining string
    if (re2::RE2::FullMatch(working_copy, valid_result_expression)) {
        entry.set_result(working_copy);
    } else {
        std::string result_empty;
        entry.set_result(result_empty);
    }

    return 0;
}
