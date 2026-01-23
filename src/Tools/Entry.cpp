#include "Entry.h"
#include "Color.h"
#include "global.h"

#if defined(_WIN32)
#include <curses.h>
#else
#include <ncurses.h>
#endif

#include <sstream>
#include <stdint.h>
#include <string_view>
#include <string>

static uint32_t currentIdentifier = 1;

namespace Tools {

Entry::Entry() : identifier_m(currentIdentifier++) {}

void Entry::SetStylized(std::string_view stylized) {
    stylizedContent_m = stylized;
}
std::string Entry::GetStylized() const {
    return stylizedContent_m;
}

void Entry::SetResult(std::string_view result) {
    result_m = result;
}
std::string Entry::GetResult() const {
    return result_m;
}

void Entry::PrintStylized() const {
    std::stringstream stream(stylizedContent_m);
    std::string token;

    int color = -1;
    char temp;
    while (std::getline(stream, token, '{')) {
        wprintw(globalState.window_mp, "%s", token.c_str());

        if (color >= 0)
            wattroff(globalState.window_mp, COLOR_PAIR(color));

        if (!stream.eof()) {
            if (stream.peek() != '}') {
                stream >> color;
                wattron(globalState.window_mp, COLOR_PAIR(color));
            }
            stream >> temp;
        }
    }

    PrintResult();
}

void Entry::PrintResult() const {
    if (!result_m.empty()) {
        wprintw(globalState.window_mp, " = ");
        wattron(globalState.window_mp, COLOR_PAIR(ColorType::Result));
        wprintw(globalState.window_mp, "%s", result_m.c_str());
        wattroff(globalState.window_mp, COLOR_PAIR(ColorType::Result));
    }
}

bool Entry::operator==(const Entry& second) const {
    return identifier_m == second.identifier_m;
}

bool Entry::operator!=(const Entry& second) const {
    return !(*this == second);
}

}
