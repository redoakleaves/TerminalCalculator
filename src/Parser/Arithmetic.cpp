#include "Arithmetic.h"

#include <re2/re2.h>

#include <cctype>
#include <deque>
#include <sstream>
#include <string>

namespace Parser {

static const re2::RE2 kArithmeticExpression("(-?\\d+(?:[\\.\\,]\\d+)?(?:[\\/*+-](?:-?\\d+(?:[\\.\\,]\\d+)?))+)");
static const re2::RE2 kDoubleSignExpression("(\\+-|-\\+|--|\\+\\+)");

static bool IsOperand(char c) {
    return c == '*' || c == '/' || c == '+' || c == '-';
}

static void ReplaceDoubleSigns(std::string& substring) {
    std::string substringCopy = substring;

    int offset = 0;
    int delta = 0;
    re2::StringPiece match;
    while (re2::RE2::PartialMatch(substringCopy.data() + offset, kDoubleSignExpression, &match)) {
        offset = match.data() - substringCopy.data();

        if (match == "+-" || match == "-+") {
            substring.replace(offset + delta, 2, "-");
        } else if (match == "--" || match == "++") {
            substring.replace(offset + delta, 2, "+");
        }

        delta--;
        offset++;
    }
}

void ArithmeticParser::ParseArithmetic(std::string& substring) {
    if (!re2::RE2::FullMatch(substring, kArithmeticExpression, nullptr)) {
        return;
    }

    ReplaceDoubleSigns(substring);

    std::deque<double> operandStack;
    std::deque<char> operationStack;

    // Parse substring char by char whilst solving multiplication and division on the way
    std::stringstream builder;
    char current = 0;
    char previous = 0;
    double tempNumber = 0;
    for (auto it = substring.begin(); it != substring.end(); ++it) {
        current = *it;

        if ((std::isdigit(current) || current == '.' || current == ',') && it + 1 != substring.end()) {
            builder << current;
        } else if (current == '-' && (IsOperand(previous) || previous == 0)) {
            builder << current;
        } else {
            if (it + 1 == substring.end()) {
                builder << current;
            }
            builder >> tempNumber;

            if (!operandStack.empty() && !operationStack.empty()) {
                const char operation = operationStack.back();
                const double operand = operandStack.back();
                if (operation == '*') {
                    operandStack.pop_back();
                    operationStack.pop_back();
                    tempNumber *= operand;
                } else if (operation == '/') {
                    operandStack.pop_back();
                    operationStack.pop_back();
                    if (tempNumber == 0) {
                        return;
                    }
                    tempNumber = operand / tempNumber;
                }
            }

            operandStack.push_back(tempNumber);
            if (!std::isdigit(current)) {
                operationStack.push_back(current);
            }

            // Reset builder
            builder.clear();
            builder.str("");
        }

        previous = current;
    }

    // Solve addition and subtraction
    double first = 0;
    double second = 0;
    char operation = 0;
    while (operandStack.size() > 1) {
        first = operandStack.front();
        operandStack.pop_front();
        second = operandStack.front();
        operandStack.pop_front();
        operation = operationStack.front();
        operationStack.pop_front();

        if (operation == '+') {
            first += second;
        } else if (operation == '-') {
            first -= second;
        } else {
            return;
        }

        operandStack.push_front(first);
    }

    // Write result to substring if existent
    if (operandStack.size() == 1 && operationStack.empty()) {
        std::stringstream stream;
        stream << operandStack.front();
        stream >> substring;
    }
}

}
