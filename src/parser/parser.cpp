#include <string>
#include <algorithm>

#include <re2/re2.h>

#include "tools/entry.h"
#include "parser.h"

namespace Parser {
    const re2::RE2 Parser::subExpression("(?:^|[^a-zA-Z\\d.,])\\(([a-zA-Z\\d.,\\^\\*\\/+-]+)\\)");
    const re2::RE2 Parser::validResultExpression("^-?\\d+(?:[\\.\\,]\\d+)?$");

    int Parser::Parse(Tools::Entry& entry, int final) {
        entry.SetStylized(entry.m_RawContent);

        // Create working copy of input and remove spaces
        std::string workingCopy = entry.m_RawContent;
        workingCopy.erase(std::remove_if(workingCopy.begin(), workingCopy.end(), ::isspace), workingCopy.end());

        // Check for commands
        int command = m_CommandParser.ParseSubstring(entry, workingCopy, final);
        if (command)
            return command;

        // Resolve function and variable definitions
        if (workingCopy.find('=') != std::string::npos) {
            int definitionResult = m_FuncParser.ParseFuncDefinition(entry, workingCopy, final);
            if (!definitionResult)
                definitionResult = m_VarParser.ParseVarDefinitions(entry, workingCopy, final);
            return definitionResult ? 0 : -1;
        }

        // Resolve function and variable usage
        m_FuncParser.ParseFuncUsage(entry, workingCopy);
        m_VarParser.ParseVarUsage(entry, workingCopy);

        // Parse pre-defined functions, subexpressions, and arithmetic until nothing changes
        int prevLength = 0;
        do {
            prevLength = workingCopy.length();
            
            m_ArithmeticParser.ParseArithmetic(entry, workingCopy);
            
            m_FuncParser.ParseConstFuncUsage(entry, workingCopy);

            // Search for subexpressions
            re2::StringPiece subexpression;
            while (re2::RE2::PartialMatch(workingCopy, subExpression, &subexpression))
            {
                std::string subexpressionString = subexpression.ToString();
                m_ArithmeticParser.ParseArithmetic(entry, subexpressionString);
                subexpressionString.replace(subexpression.data() - workingCopy.data() - 1, subexpression.length() + 2, subexpressionString);
            }
        } while (prevLength != workingCopy.length());

        // Check remaining string
        if (re2::RE2::FullMatch(workingCopy, validResultExpression)) {
            entry.SetResult(workingCopy);
        } else {
            std::string resultEmpty;
            entry.SetResult(resultEmpty);
            return -1;
        }

        return 0;
    }
}
