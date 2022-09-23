#ifndef _COLOR_H_
#define _COLOR_H_

#if defined(_WIN64)
#include <curses.h>
#else
#include <ncurses.h>
#endif

#define COLOR_RESULT 1

void init_color_defs();

#endif
