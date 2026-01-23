#pragma once

#include "Entry.h"

#if defined(_WIN32)
#include <curses.h>
#else
#include <ncurses.h>
#endif

#include <iterator>
#include <list>

namespace Tools {

class State {
public:
    std::list<Entry> history_m;
    std::list<Entry>::iterator activeEntry_m;

    int screenMaxX_m;
    int screenMaxY_m;

    WINDOW* window_mp;

    int windowMaxX_m;
    int windowMaxY_m;
    int cursorX_m;
    int cursorY_m;

    bool useDeg_m;

    State();

    void Init();

    void CreateNewEntry();
    Entry& GetActiveEntry();
    Entry& GetLatestEntry();
    void JumpToLatestEntry();
};

}
