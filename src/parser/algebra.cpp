#include <sstream>
#include <regex>
#include <math.h>

#include "algebra.h"

static const std::regex exponentExpression("((?:(?:(?:^|[\\*\\/\\+\\-])(?=-))-)?\\d+(?:[\\.\\,]\\d+)?)(\\^)(-?\\d+(?:[\\.\\,]\\d+)?)");
static const std::regex muldivExpression("((?:(?:(?:^|[\\*\\/\\+\\-])(?=-))-)?\\d+(?:[\\.\\,]\\d+)?)([\\*\\/])(-?\\d+(?:[\\.\\,]\\d+)?)");
static const std::regex addsubExpression("((?:(?:(?:^|[\\*\\/\\+\\-])(?=-))-)?\\d+(?:[\\.\\,]\\d+)?)([\\+\\-])(-?\\d+(?:[\\.\\,]\\d+)?)");

static const std::regex operations[3] = {exponentExpression, muldivExpression, addsubExpression};

void replace_double_signs(std::string& substring) {
    std::smatch match;

    while (std::regex_search(substring, match, std::regex("(?:^|[\\^*\\/+\\=])(--)"))) {
        if (match.length() > 2)
            substring.replace(match.position() + 1, 2, "");
        else
            substring.replace(match.position(), 2, "");
    }

    substring = std::regex_replace(substring, std::regex("(--)"), "+");
}

void parse_algebra(Entry& entry, std::string& substring) {
    std::smatch match;
    std::stringstream stream;

    double temp_number;
    double result;

    replace_double_signs(substring);

    for (int i = 0; i < 3; i++) {
        while (std::regex_search(substring, match, operations[i])) {
            std::string first_string = match[1].str();
            std::string second_string = match[3].str();

            if (first_string.length() > 2 && first_string.at(1) == '-')
                first_string.erase(0, 1);
            if (second_string.length() > 2 && second_string.at(1) == '-')
                second_string.erase(0, 1);

            stream.str(first_string);
            stream >> result;
            stream.clear();
            stream.str(second_string);
            stream >> temp_number;
            stream.clear();

            switch (i)
            {
            case 0:
                result = pow(result, temp_number);
                break;

            case 1:
                if (match[2].str().compare("*") == 0)
                    result *= temp_number;
                else
                    result /= temp_number;
                break;

            case 2:
                if (match[2].str().compare("+") == 0)
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

            replace_double_signs(substring);
        }
    }

    replace_double_signs(substring);
}
