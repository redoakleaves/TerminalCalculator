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

static uint32_t currentIdentifier = 1;

namespace Tools
{
    Entry::Entry() {
        m_Identifier = currentIdentifier++;

        m_Next = nullptr;
        m_Prev = nullptr;
    }
    Entry::~Entry() {
        if (m_Next)
            delete m_Next;
    }

    void Entry::SetStylized(std::string& stylized) {
        m_StylizedContent = stylized;
    }
    std::string Entry::GetStylized() {
        return m_StylizedContent;
    }

    void Entry::SetResult(std::string& result) {
        m_Result = result;
    }
    std::string Entry::GetResult() {
        return m_Result;
    }

    void Entry::PrintStylized() {
        std::stringstream stream(m_StylizedContent);
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

        PrintResult();
    }
    void Entry::PrintResult() {
        if (!m_Result.empty()) {
            printw(" = ");
            attron(COLOR_PAIR(COLOR_RESULT));
            printw("%s", m_Result.c_str());
            attroff(COLOR_PAIR(COLOR_RESULT));
        }
    }
}
