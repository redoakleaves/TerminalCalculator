#include <stdio.h>
#include <string>

#if defined(_WIN32)
#include <curses.h>
#else
#include <ncurses.h>
#endif

#include "global.h"

#include "tools/color.h"
#include "tools/config.h"
#include "tools/state.h"

#include "Parser/Commands.h"
#include "Parser/Parser.h"

#define EXIT -1
#define REDRAW 1
#define NO_REDRAW 2

Tools::Config globalconfig;
Tools::State globalstate;

int GetPrefixLength() {
    return snprintf(nullptr, 0, "%u", globalstate.m_Latest->m_Identifier) + 2;
}

void PrintPrefix() {
    wprintw(globalstate.m_Window, "%u: ", globalstate.m_Latest->m_Identifier);
}
void PrintTitle() {
    move(0, 0);
    std::string showVersionSetting = globalconfig.GetValue("general.show_version");
    if (showVersionSetting.empty() || showVersionSetting == "true") {
        printw("Terminal Calculator %s\n", PROJECT_VER);
    } else {
        printw("Terminal Calculator\n");
    }
}
void PrintFooter() {
    move(globalstate.m_ScreenMaxY - 1, 0);
    printw("UpArrow: Up in history DownArrow: Down in history");
}

int HandleInput(Tools::Entry* entry, int input) {
    switch (input) {
        case -1:
            return NO_REDRAW;

        // SIGINT
        case 3:
            return EXIT;

        // Enter
        case 10:
        case 13:
            if (!entry->m_RawContent.empty())
                return 0;
            return NO_REDRAW;

        // Handle resize
        case KEY_RESIZE:
            move(globalstate.m_ScreenMaxY - 1, 0);
            clrtoeol();
            getmaxyx(stdscr, globalstate.m_ScreenMaxY, globalstate.m_ScreenMaxX);
            PrintFooter();
            wresize(globalstate.m_Window, globalstate.m_ScreenMaxY - 2, globalstate.m_ScreenMaxX);
            getmaxyx(globalstate.m_Window, globalstate.m_WindowMaxY, globalstate.m_WindowMaxX);
            refresh();
            return NO_REDRAW;

        // Move cursor left or right
        case KEY_LEFT:
            if (globalstate.m_CursorX > GetPrefixLength())
                globalstate.m_CursorX--;
            return NO_REDRAW;
        case KEY_RIGHT:
            if (globalstate.m_CursorX < GetPrefixLength() + entry->m_RawContent.length())
                globalstate.m_CursorX++;
            return NO_REDRAW;

        // Browse history
        case KEY_UP:
            if (globalstate.m_Current->m_Prev) {
                globalstate.m_Current = globalstate.m_Current->m_Prev;
                globalstate.m_CursorX = GetPrefixLength() + globalstate.m_Current->m_RawContent.length();
            }
            break;
        case KEY_DOWN:
            if (globalstate.m_Current->m_Next) {
                globalstate.m_Current = globalstate.m_Current->m_Next;
                globalstate.m_CursorX = GetPrefixLength() + globalstate.m_Current->m_RawContent.length();
            }
            break;

        // Backspace
        case 8:
        case 127:
        case KEY_BACKSPACE:
            if (entry->m_RawContent.length() > 0) {
                if (globalstate.m_Current != globalstate.m_Latest) {
                    globalstate.m_Latest->m_RawContent = globalstate.m_Current->m_RawContent;
                    entry = globalstate.m_Current = globalstate.m_Latest;
                }
                entry->m_RawContent.erase(globalstate.m_CursorX - GetPrefixLength() - 1, 1);
                globalstate.m_CursorX--;
            }
            break;

        // Delete
        case KEY_DC:
            if (entry->m_RawContent.length() > 0) {
                if (globalstate.m_Current != globalstate.m_Latest) {
                    globalstate.m_Latest->m_RawContent = globalstate.m_Current->m_RawContent;
                    entry = globalstate.m_Current = globalstate.m_Latest;
                }
                entry->m_RawContent.erase(globalstate.m_CursorX - GetPrefixLength(), 1);
            }
            break;

        default:
            if (globalstate.m_Current != globalstate.m_Latest) {
                globalstate.m_Latest->m_RawContent = globalstate.m_Current->m_RawContent;
                entry = globalstate.m_Current = globalstate.m_Latest;
            }
            if (globalstate.m_CursorX < GetPrefixLength() + entry->m_RawContent.length())
                entry->m_RawContent.insert(globalstate.m_CursorX - GetPrefixLength(), 1, (char)input);
            else
                entry->m_RawContent.push_back((char)input);
            globalstate.m_CursorX++;
    }

    return REDRAW;
}

int main(int argc, char* argv[]) {
    // Load config if existent
    globalconfig.LoadConfig();

    // Init state
    globalstate.Init();

    // Init curses
    initscr();
    noecho();
    raw();

    // Init colors
    Tools::init_color_defs(globalconfig);

    getmaxyx(stdscr, globalstate.m_ScreenMaxY, globalstate.m_ScreenMaxX);

    // Setup window
    globalstate.m_Window = newwin(globalstate.m_ScreenMaxY - 2, globalstate.m_ScreenMaxX, 1, 0);
    getmaxyx(globalstate.m_Window, globalstate.m_WindowMaxY, globalstate.m_WindowMaxX);
    keypad(globalstate.m_Window, 1);
    scrollok(globalstate.m_Window, 1);

    PrintTitle();
    PrintFooter();
    refresh();

    globalstate.m_CursorY = 0;
    globalstate.m_CursorX = 0;

    Parser::Parser parser;
    int resultInput = REDRAW;
    int resultParse = -1;
    while (1) {
        globalstate.m_CursorX += GetPrefixLength();

        if (globalstate.m_CursorY > globalstate.m_WindowMaxY - 1)
            globalstate.m_CursorY = globalstate.m_WindowMaxY - 1;

        do {
            if (resultInput == REDRAW) {
                parser.Parse(*globalstate.m_Current);

                wmove(globalstate.m_Window, globalstate.m_CursorY, 0);
                wclrtoeol(globalstate.m_Window);
                wmove(globalstate.m_Window, globalstate.m_CursorY, 0);
                PrintPrefix();

                globalstate.m_Current->PrintStylized();
            }
            wmove(globalstate.m_Window, globalstate.m_CursorY, globalstate.m_CursorX);
            wrefresh(globalstate.m_Window);

            resultInput = HandleInput(globalstate.m_Current, wgetch(globalstate.m_Window));
        } while (resultInput > 0);

        // Final parse for var definitions etc.
        resultParse = parser.Parse(*globalstate.m_Current, 1);

        if (resultParse == Parser::Command::Exit || resultInput == EXIT) {
            break;
        }

        // Add newline to the end of every line
        wmove(globalstate.m_Window, globalstate.m_CursorY, globalstate.m_CursorX + globalstate.m_Current->GetResult().length() + 3);
        waddch(globalstate.m_Window, '\n');

        globalstate.m_CursorY++;
        globalstate.m_CursorX = 0;
        resultInput = REDRAW;

        // Create new entry in history
        globalstate.CreateNewEntry();
    }

    endwin();

    return EXIT_SUCCESS;
}
