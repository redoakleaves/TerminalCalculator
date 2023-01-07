#ifndef _STATE_H_
#define _STATE_H_

#if defined(_WIN32)
#include <curses.h>
#else
#include <ncurses.h>
#endif

#include "entry.h"

namespace Tools
{
    class State {
    public:
        Entry* m_History;
        Entry* m_Current;
        Entry* m_Latest;

        int m_ScreenMaxX;
        int m_ScreenMaxY;

        WINDOW* m_Window;

        int m_WindowMaxX;
        int m_WindowMaxY;
        int m_CursorX;
        int m_CursorY;

        int m_UseDeg;

        State();
        ~State();

        void Init();

        void CreateNewEntry();
    };
}

#endif
