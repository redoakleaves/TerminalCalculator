#include <stdio.h>
#include <string>

#if defined(_WIN32)
#include <curses.h>
#else
#include <ncurses.h>
#endif

#include "config.h"
#include "state.h"
#include "tools/color.h"

#include "parser/parser.h"
#include "parser/commands.h"

#define EXIT -1
#define REDRAW 1
#define NO_REDRAW 2

static Config globalconfig;
static State globalstate;

int prefix_length() {
    char buffer[10];
    return snprintf(buffer, 10, "%u", globalstate.latest->identifier) + 2;
}
void print_prefix() {
    printw("%u: ", globalstate.latest->identifier);
}

void print_footer() {
    move(globalstate.max_y - 1, 0);
    printw("UpArrow: Up in history DownArrow: Down in history");
}

int handle_input(Entry* entry, int input) {
    switch (input) {
        case -1:
            return NO_REDRAW;

        // SIGINT
        case 3:
            return EXIT;

        // Enter
        case 10:
        case 13:
            if (!entry->raw_content.empty())
                return 0;
            return NO_REDRAW;

        // Handle resize
        case KEY_RESIZE:
            move(globalstate.max_y - 1, 0);
            clrtoeol();
            getmaxyx(stdscr, globalstate.max_y, globalstate.max_x);
            print_footer();
            return NO_REDRAW;

        // Move cursor left or right
        case KEY_LEFT:
            if (globalstate.cursor_x > prefix_length())
                globalstate.cursor_x--;
            return NO_REDRAW;
        case KEY_RIGHT:
            if (globalstate.cursor_x < prefix_length() + entry->raw_content.length())
                globalstate.cursor_x++;
            return NO_REDRAW;

        // Browse history
        case KEY_UP:
            if (globalstate.current->prev) {
                globalstate.current = globalstate.current->prev;
                globalstate.cursor_x = prefix_length() + globalstate.current->raw_content.length();
            }
            break;
        case KEY_DOWN:
            if (globalstate.current->next) {
                globalstate.current = globalstate.current->next;
                globalstate.cursor_x = prefix_length() + globalstate.current->raw_content.length();
            }
            break;

        // Backspace
        case 8:
        case 127:
        case KEY_BACKSPACE:
            if (entry->raw_content.length() > 0) {
                if (globalstate.current != globalstate.latest) {
                    globalstate.latest->raw_content = globalstate.current->raw_content;
                    entry = globalstate.current = globalstate.latest;
                }
                entry->raw_content.erase(globalstate.cursor_x - prefix_length() - 1, 1);
                globalstate.cursor_x--;
            }
            break;

        // Delete
        case KEY_DC:
            if (entry->raw_content.length() > 0) {
                if (globalstate.current != globalstate.latest) {
                    globalstate.latest->raw_content = globalstate.current->raw_content;
                    entry = globalstate.current = globalstate.latest;
                }
                entry->raw_content.erase(globalstate.cursor_x - prefix_length(), 1);
            }
            break;

        default:
            if (globalstate.current != globalstate.latest) {
                globalstate.latest->raw_content = globalstate.current->raw_content;
                entry = globalstate.current = globalstate.latest;
            }
            if (globalstate.cursor_x < prefix_length() + entry->raw_content.length())
                entry->raw_content.insert(globalstate.cursor_x - prefix_length(), 1, (char)input);
            else
                entry->raw_content.push_back((char)input);
            globalstate.cursor_x++;
    }

    return REDRAW;
}

int main(int argc, char* argv[]) {
    // Load config if existent
    globalconfig.load_config();

    // Init curses
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    raw();
    
    // Init colors
    init_color_defs(globalconfig);

    getmaxyx(stdscr, globalstate.max_y, globalstate.max_x);

    print_footer();

    // Print title
    move(0, 0);
    std::string show_version_setting = globalconfig.get_value("general.show_version");
    if (show_version_setting.empty() || show_version_setting == "true") {
        printw("Terminal Calculator %s\n", PROJECT_VER);
    } else {
        printw("Terminal Calculator\n");
    }

    int result_input = REDRAW;
    int result_parse = -1;
    getyx(stdscr, globalstate.cursor_y, globalstate.cursor_x);
    while (1) {
        globalstate.cursor_x += prefix_length();

        do {
            if (result_input == REDRAW) {
                result_parse = parse(*globalstate.current);

                move(globalstate.cursor_y, 0);
                clrtoeol();
                move(globalstate.cursor_y, 0);
                print_prefix();
                globalstate.current->print_stylized();
            }
            move(globalstate.cursor_y, globalstate.cursor_x);
            refresh();

            result_input = handle_input(globalstate.current, getch());
        } while (result_input > 0);

        if (result_parse == Commands::Exit || result_input == EXIT)
            break;

        // Final parse for var definitions etc.
        parse(*globalstate.current, 1);

        globalstate.cursor_y++;
        globalstate.cursor_x = 0;
        result_input = REDRAW;

        // Create new entry in history
        globalstate.create_new_entry();
    }

    endwin();

    return EXIT_SUCCESS;
}
