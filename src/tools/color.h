#ifndef _COLOR_H_
#define _COLOR_H_

#if defined(_WIN64)
#include <curses.h>
#else
#include <ncurses.h>
#endif

#include "config.h"

#define COLOR_RESULT 1
#define COLOR_COMMAND 2

void init_color_defs(Config& config);

#endif
