#include <sstream>
#include <string>
#include <regex>
#include <cmath>

#include <re2/re2.h>

#include "tools/entry.h"
#include "algebra.h"

static const std::regex exponentExpression("((?:(?:(?:^|[\\*\\/\\+\\-])(?=-))-)?\\d+(?:[\\.\\,]\\d+)?)(\\^)(-?\\d+(?:[\\.\\,]\\d+)?)");
static const std::regex muldivExpression("((?:(?:(?:^|[\\*\\/\\+\\-])(?=-))-)?\\d+(?:[\\.\\,]\\d+)?)([\\*\\/])(-?\\d+(?:[\\.\\,]\\d+)?)");
static const std::regex addsubExpression("((?:(?:(?:^|[\\*\\/\\+\\-])(?=-))-)?\\d+(?:[\\.\\,]\\d+)?)([\\+\\-])(-?\\d+(?:[\\.\\,]\\d+)?)");

static const re2::RE2 doubleSignExpression("(?:^|[\\^*\\/+\\=])(--)");

static const std::regex operations[3] = {exponentExpression, muldivExpression, addsubExpression};

static void replace_double_signs(std::string& substring) {
    re2::StringPiece match;
    while (re2::RE2::PartialMatch(substring, doubleSignExpression, &match)) {
        if (match.length() > 2)
            substring.replace((match.data() - substring.data()) + 1, 2, "");
        else
            substring.replace(match.data() - substring.data(), 2, "");
    }

    substring = std::regex_replace(substring, std::regex("(--)"), "+");
}

void parse_algebra(Tools::Entry& entry, std::string& substring) {
    std::smatch match;
    std::stringstream stream;

    double temp_number;
    double result;

    replace_double_signs(substring);

    for (int i = 0; i < 3; i++) {
        while (std::regex_search(substring, match, operations[i])) {
            std::string first_string = match[1].str();
            std::string operator_string = match[2].str();
            std::string second_string = match[3].str();

            if (first_string.length() > 2 && first_string.at(1) == '-')
                first_string.erase(0, 1);
            if (second_string.length() > 2 && second_string.at(1) == '-')
                second_string.erase(0, 1);

            result = std::stod(first_string);
            temp_number = std::stod(second_string);

            switch (i)
            {
            case 0:
                result = std::pow(result, temp_number);
                break;

            case 1:
                if (operator_string.compare("*") == 0)
                    result *= temp_number;
                else
                    result /= temp_number;
                break;

            case 2:
                if (operator_string.compare("+") == 0)
                    result += temp_number;
                else
                    result -= temp_number;
                break;

            default:
                break;
            }

            stream.str("");
            stream << result;
            substring.replace(match.position(), match.length(), stream.str());
            stream.clear();

            replace_double_signs(substring);
        }
    }

    replace_double_signs(substring);
}
