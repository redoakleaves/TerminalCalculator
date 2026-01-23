#include "global.h"
#include "Parser/Commands.h"
#include "Parser/Parser.h"
#include "Tools/Color.h"
#include "Tools/Config.h"
#include "Tools/State.h"

#if defined(_WIN32)
#include <curses.h>
#else
#include <ncurses.h>
#endif

#include <iterator>
#include <stdio.h>
#include <string>

#define EXIT -1
#define REDRAW 1
#define NO_REDRAW 2

Tools::Config globalConfig;
Tools::State globalState;

int GetPrefixLength() {
    return snprintf(nullptr, 0, "%u", globalState.history_m.back().identifier_m) + 2;
}

void PrintPrefix() {
    wprintw(globalState.window_mp, "%u: ", globalState.history_m.back().identifier_m);
}
void PrintTitle() {
    move(0, 0);
    const std::string showVersionSetting = globalConfig.GetValue("general.show_version");
    if (showVersionSetting.empty() || showVersionSetting == "true") {
        printw("Terminal Calculator %s\n", PROJECT_VER);
    } else {
        printw("Terminal Calculator\n");
    }
}
void PrintFooter() {
    move(globalState.screenMaxY_m - 1, 0);
    printw("UpArrow: Up in history DownArrow: Down in history");
}

int HandleInput(int input) {
    switch (input) {
        case -1:
            return NO_REDRAW;

        // SIGINT
        case 3:
            return EXIT;

        // Enter
        case 10:
            [[fallthrough]];
        case 13:
            if (!globalState.GetActiveEntry().rawContent_m.empty()) {
                return 0;
            }
            return NO_REDRAW;

        // Handle resize
        case KEY_RESIZE:
            move(globalState.screenMaxY_m - 1, 0);
            clrtoeol();
            getmaxyx(stdscr, globalState.screenMaxY_m, globalState.screenMaxX_m);
            PrintFooter();
            wresize(globalState.window_mp, globalState.screenMaxY_m - 2, globalState.screenMaxX_m);
            getmaxyx(globalState.window_mp, globalState.windowMaxY_m, globalState.windowMaxX_m);
            refresh();
            return NO_REDRAW;

        // Move cursor left or right
        case KEY_LEFT:
            if (globalState.cursorX_m > GetPrefixLength()) {
                globalState.cursorX_m--;
            }
            return NO_REDRAW;
        case KEY_RIGHT:
            if (globalState.cursorX_m < GetPrefixLength() + globalState.GetActiveEntry().rawContent_m.length()) {
                globalState.cursorX_m++;
            }
            return NO_REDRAW;

        // Browse history
        case KEY_UP:
            if (globalState.activeEntry_m != globalState.history_m.begin()) {
                globalState.activeEntry_m = std::prev(globalState.activeEntry_m);
                globalState.cursorX_m = GetPrefixLength() + globalState.GetActiveEntry().rawContent_m.length();
            }
            break;
        case KEY_DOWN:
            if (globalState.GetActiveEntry() != globalState.GetLatestEntry()) {
                globalState.activeEntry_m = std::next(globalState.activeEntry_m);
                globalState.cursorX_m = GetPrefixLength() + globalState.GetActiveEntry().rawContent_m.length();
            }
            break;

        // Backspace
        case 8:
            [[fallthrough]];
        case 127:
            [[fallthrough]];
        case KEY_BACKSPACE:
            if (globalState.GetActiveEntry().rawContent_m.length() > 0) {
                if (globalState.GetActiveEntry() != globalState.GetLatestEntry()) {
                    globalState.GetLatestEntry().rawContent_m = globalState.GetActiveEntry().rawContent_m;
                    globalState.JumpToLatestEntry();
                }
                globalState.GetActiveEntry().rawContent_m.erase(globalState.cursorX_m - GetPrefixLength() - 1, 1);
                globalState.cursorX_m--;
            }
            break;

        // Delete
        case KEY_DC:
            if (globalState.GetActiveEntry().rawContent_m.length() > 0) {
                if (globalState.GetActiveEntry() != globalState.GetLatestEntry()) {
                    globalState.GetLatestEntry().rawContent_m = globalState.GetActiveEntry().rawContent_m;
                    globalState.JumpToLatestEntry();
                }
                globalState.GetActiveEntry().rawContent_m.erase(globalState.cursorX_m - GetPrefixLength(), 1);
            }
            break;

        default:
            if (globalState.GetActiveEntry() != globalState.GetLatestEntry()) {
                globalState.GetLatestEntry().rawContent_m = globalState.GetActiveEntry().rawContent_m;
                globalState.JumpToLatestEntry();
            }
            if (globalState.cursorX_m < GetPrefixLength() + globalState.GetActiveEntry().rawContent_m.length()) {
                globalState.GetActiveEntry().rawContent_m.insert(globalState.cursorX_m - GetPrefixLength(), 1, (char)input);
            } else {
                globalState.GetActiveEntry().rawContent_m.push_back((char)input);
            }
            globalState.cursorX_m++;
    }

    return REDRAW;
}

int main(int argc, char* argv[]) {
    // Load config if existent
    globalConfig.LoadConfig();

    // Init state
    globalState.Init();

    // Init curses
    initscr();
    noecho();
    raw();

    // Init colors
    Tools::Color::InitializeColorDefinitions(globalConfig);

    getmaxyx(stdscr, globalState.screenMaxY_m, globalState.screenMaxX_m);

    // Setup window
    globalState.window_mp = newwin(globalState.screenMaxY_m - 2, globalState.screenMaxX_m, 1, 0);
    getmaxyx(globalState.window_mp, globalState.windowMaxY_m, globalState.windowMaxX_m);
    keypad(globalState.window_mp, 1);
    scrollok(globalState.window_mp, 1);

    PrintTitle();
    PrintFooter();
    refresh();

    globalState.cursorY_m = 0;
    globalState.cursorX_m = 0;

    Parser::Parser parser;
    int resultInput = REDRAW;
    int resultParse = -1;
    while (1) {
        globalState.cursorX_m += GetPrefixLength();

        if (globalState.cursorY_m > globalState.windowMaxY_m - 1)
            globalState.cursorY_m = globalState.windowMaxY_m - 1;

        do {
            if (resultInput == REDRAW) {
                parser.Parse(globalState.GetActiveEntry());

                wmove(globalState.window_mp, globalState.cursorY_m, 0);
                wclrtoeol(globalState.window_mp);
                wmove(globalState.window_mp, globalState.cursorY_m, 0);
                PrintPrefix();

                globalState.GetActiveEntry().PrintStylized();
            }
            wmove(globalState.window_mp, globalState.cursorY_m, globalState.cursorX_m);
            wrefresh(globalState.window_mp);

            resultInput = HandleInput(wgetch(globalState.window_mp));
        } while (resultInput > 0);

        // Final parse for var definitions etc.
        resultParse = parser.Parse(globalState.GetActiveEntry(), true);

        if (resultParse == Parser::Command::Exit || resultInput == EXIT) {
            break;
        }

        // Add newline to the end of every line
        wmove(globalState.window_mp, globalState.cursorY_m, globalState.cursorX_m + globalState.GetActiveEntry().GetResult().length() + 3);
        waddch(globalState.window_mp, '\n');

        globalState.cursorY_m++;
        globalState.cursorX_m = 0;
        resultInput = REDRAW;

        // Create new entry in history
        globalState.CreateNewEntry();
    }

    endwin();

    return EXIT_SUCCESS;
}
