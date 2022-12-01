#include <string>
#include <sstream>
#include <deque>
#include <cmath>

#include <re2/re2.h>

#include "tools/entry.h"
#include "arithmetic.h"

static const re2::RE2 arithmeticExpression("(-?\\d+(?:[\\.\\,]\\d+)?(?:[\\/*+-](?:-?\\d+(?:[\\.\\,]\\d+)?))+)");
static const re2::RE2 doubleSignExpression("(\\+-|-\\+|--|\\+\\+)");

static int is_operand(char& string) {
    return string == '*' || string == '/' || string == '+' || string == '-';
}

static void replace_double_signs(std::string& substring) {
    std::string substring_copy = substring;

    int offset = 0;
    int delta = 0;
    re2::StringPiece match;
    while (re2::RE2::PartialMatch(substring_copy.data() + offset, doubleSignExpression, &match)) {
        offset = match.data() - substring_copy.data();

        if (match == "+-" || match == "-+")
            substring.replace(offset + delta, 2, "-");
        else if (match == "--" || match == "++")
            substring.replace(offset + delta, 2, "+");

        delta--;
        offset++;
    }
}

void parse_arithmetic(Tools::Entry& entry, std::string& substring) {
    if (!re2::RE2::FullMatch(substring, arithmeticExpression, nullptr))
        return;

    replace_double_signs(substring);

    std::deque<double> operand_stack;
    std::deque<char> operation_stack;

    // Parse substring char by char whilst solving multiplication and division on the way
    std::stringstream builder;
    char current = 0;
    char previous = 0;
    double temp_number = 0;
    for (auto it = substring.begin(); it != substring.end(); ++it) {
        current = *it;

        if ((isdigit(current) || current == '.' || current == ',') && it + 1 != substring.end()) {
            builder << current;
        } else if (current == '-' && (is_operand(previous) || previous == 0)) {
            builder << current;
        } else {
            if (it + 1 == substring.end())
                builder << current;
            builder >> temp_number;

            if (!operand_stack.empty() && !operation_stack.empty()) {
                char operation = operation_stack.back();
                double operand = operand_stack.back();
                if (operation == '*') {
                    operand_stack.pop_back();
                    operation_stack.pop_back();
                    temp_number *= operand;
                } else if (operation == '/') {
                    operand_stack.pop_back();
                    operation_stack.pop_back();
                    temp_number = operand / temp_number;
                }
            }

            operand_stack.push_back(temp_number);
            if (!isdigit(current))
                operation_stack.push_back(current);

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
    while (operand_stack.size() > 1) {
        first = operand_stack.front();
        operand_stack.pop_front();
        second = operand_stack.front();
        operand_stack.pop_front();
        operation = operation_stack.front();
        operation_stack.pop_front();

        if (operation == '+') {
            first += second;
        } else if (operation == '-') {
            first -= second;
        } else {
            return;
        }

        operand_stack.push_front(first);
    }

    // Write result to substring if existent
    if (operand_stack.size() == 1 && operation_stack.empty()) {
        std::stringstream stream;
        stream << operand_stack.front();
        stream >> substring;
    }
}
