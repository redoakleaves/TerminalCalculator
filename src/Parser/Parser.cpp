#include "Parser.h"
#include "Arithmetic.h"
#include "Commands.h"
#include "Tools/Entry.h"

#include <re2/re2.h>

#include <algorithm>
#include <optional>
#include <string>

namespace Parser {

static const re2::RE2 kSubExpression("(?:^|[^a-zA-Z\\d.,])\\(([a-zA-Z\\d.,\\^\\*\\/+-]+)\\)");
static const re2::RE2 kValidResultExpression("^-?\\d+(?:[\\.\\,]\\d+)?$");

int Parser::Parse(Tools::Entry& entry, bool final) {
    entry.SetStylized(entry.rawContent_m);

    // Create working copy of input and remove spaces
    std::string workingCopy = entry.rawContent_m;
    workingCopy.erase(std::remove_if(workingCopy.begin(), workingCopy.end(), ::isspace), workingCopy.end());

    // Check for commands
    const std::optional<Command> command = CommandParser::ParseSubstring(entry, workingCopy, final);
    if (command) {
        return command.value();
    }

    // Resolve function and variable definitions
    if (workingCopy.find('=') != std::string::npos) {
        bool definitionResult = funcParser_m.ParseFuncDefinition(workingCopy, final);
        if (!definitionResult) {
            definitionResult = varParser_m.ParseVarDefinitions(workingCopy, final);
        }
        return definitionResult ? 0 : -1;
    }

    // Resolve function and variable usage
    funcParser_m.ParseFuncUsage(workingCopy);
    varParser_m.ParseVarUsage(workingCopy);

    // Parse pre-defined functions, subexpressions, and arithmetic until nothing changes
    size_t prevLength = 0;
    do {
        prevLength = workingCopy.length();

        ArithmeticParser::ParseArithmetic(workingCopy);

        funcParser_m.ParseConstFuncUsage(workingCopy);

        // Search for subexpressions
        re2::StringPiece subexpression;
        while (re2::RE2::PartialMatch(workingCopy, kSubExpression, &subexpression)) {
            std::string subexpressionString = subexpression.ToString();
            ArithmeticParser::ParseArithmetic(subexpressionString);
            workingCopy.replace(subexpression.data() - workingCopy.data() - 1, subexpression.length() + 2, subexpressionString);
        }
    } while (prevLength != workingCopy.length());

    // Check remaining string
    if (re2::RE2::FullMatch(workingCopy, kValidResultExpression)) {
        entry.SetResult(workingCopy);
    } else {
        std::string resultEmpty;
        entry.SetResult(resultEmpty);
        return -1;
    }

    return 0;
}

}
