#include <stdint.h>
#include <string>
#include <sstream>

#if defined(_WIN32)
#include <curses.h>
#else
#include <ncurses.h>
#endif

#include "color.h"
#include "entry.h"

static uint32_t current_identifier = 1;

namespace Tools
{
    Entry::Entry() {
        identifier = current_identifier++;

        next = nullptr;
        prev = nullptr;
    }
    Entry::~Entry() {
        if (next)
            delete next;
    }

    void Entry::set_stylized(std::string& stylized) {
        stylized_content = stylized;
    }
    void Entry::set_result(std::string& result) {
        this->result = result;
    }
    void Entry::print_stylized() {
        std::stringstream stream(stylized_content);
        std::string token;

        int color = -1;
        char temp;
        while (std::getline(stream, token, '{')) {
            printw("%s", token.c_str());

            if (color >= 0)
                attroff(COLOR_PAIR(color));

            if (!stream.eof()) {
                if (stream.peek() != '}') {
                    stream >> color;
                    attron(COLOR_PAIR(color));
                }
                stream >> temp;
            }
        }

        print_result();
    }
    void Entry::print_result() {
        if (!result.empty()) {
            printw(" = ");
            attron(COLOR_PAIR(COLOR_RESULT));
            printw("%s", result.c_str());
            attroff(COLOR_PAIR(COLOR_RESULT));
        }
    }
}
