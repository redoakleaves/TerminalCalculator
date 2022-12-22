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

#include "parser/parser.h"
#include "parser/commands.h"

#define EXIT -1
#define REDRAW 1
#define NO_REDRAW 2

Tools::Config globalconfig;
Tools::State globalstate;

int prefix_length() {
    char buffer[10];
    return snprintf(buffer, 10, "%u", globalstate.m_Latest->m_Identifier) + 2;
}
void print_prefix() {
    printw("%u: ", globalstate.m_Latest->m_Identifier);
}

void print_footer() {
    move(globalstate.m_MaxY - 1, 0);
    printw("UpArrow: Up in history DownArrow: Down in history");
}

int handle_input(Tools::Entry* entry, int input) {
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
            move(globalstate.m_MaxY - 1, 0);
            clrtoeol();
            getmaxyx(stdscr, globalstate.m_MaxY, globalstate.m_MaxX);
            print_footer();
            return NO_REDRAW;

        // Move cursor left or right
        case KEY_LEFT:
            if (globalstate.m_CursorX > prefix_length())
                globalstate.m_CursorX--;
            return NO_REDRAW;
        case KEY_RIGHT:
            if (globalstate.m_CursorX < prefix_length() + entry->m_RawContent.length())
                globalstate.m_CursorX++;
            return NO_REDRAW;

        // Browse history
        case KEY_UP:
            if (globalstate.m_Current->m_Prev) {
                globalstate.m_Current = globalstate.m_Current->m_Prev;
                globalstate.m_CursorX = prefix_length() + globalstate.m_Current->m_RawContent.length();
            }
            break;
        case KEY_DOWN:
            if (globalstate.m_Current->m_Next) {
                globalstate.m_Current = globalstate.m_Current->m_Next;
                globalstate.m_CursorX = prefix_length() + globalstate.m_Current->m_RawContent.length();
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
                entry->m_RawContent.erase(globalstate.m_CursorX - prefix_length() - 1, 1);
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
                entry->m_RawContent.erase(globalstate.m_CursorX - prefix_length(), 1);
            }
            break;

        default:
            if (globalstate.m_Current != globalstate.m_Latest) {
                globalstate.m_Latest->m_RawContent = globalstate.m_Current->m_RawContent;
                entry = globalstate.m_Current = globalstate.m_Latest;
            }
            if (globalstate.m_CursorX < prefix_length() + entry->m_RawContent.length())
                entry->m_RawContent.insert(globalstate.m_CursorX - prefix_length(), 1, (char)input);
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
    keypad(stdscr, TRUE);
    noecho();
    raw();
    
    // Init colors
    Tools::init_color_defs(globalconfig);

    getmaxyx(stdscr, globalstate.m_MaxY, globalstate.m_MaxX);

    print_footer();

    // Print title
    move(0, 0);
    std::string show_version_setting = globalconfig.GetValue("general.show_version");
    if (show_version_setting.empty() || show_version_setting == "true") {
        printw("Terminal Calculator %s\n", PROJECT_VER);
    } else {
        printw("Terminal Calculator\n");
    }

    Parser::Parser parser;
    int result_input = REDRAW;
    int result_parse = -1;
    getyx(stdscr, globalstate.m_CursorY, globalstate.m_CursorX);
    while (1) {
        globalstate.m_CursorX += prefix_length();

        do {
            if (result_input == REDRAW) {
                parser.Parse(*globalstate.m_Current);

                move(globalstate.m_CursorY, 0);
                clrtoeol();
                move(globalstate.m_CursorY, 0);
                print_prefix();
                globalstate.m_Current->PrintStylized();
            }
            move(globalstate.m_CursorY, globalstate.m_CursorX);
            refresh();

            result_input = handle_input(globalstate.m_Current, getch());
        } while (result_input > 0);

        // Final parse for var definitions etc.
        result_parse = parser.Parse(*globalstate.m_Current, 1);

        if (result_parse == Commands::Exit || result_input == EXIT)
            break;

        globalstate.m_CursorY++;
        globalstate.m_CursorX = 0;
        result_input = REDRAW;

        // Create new entry in history
        globalstate.CreateNewEntry();
    }

    endwin();

    return EXIT_SUCCESS;
}
