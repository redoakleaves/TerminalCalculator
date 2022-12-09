#ifndef _STATE_H_
#define _STATE_H_

#include "entry.h"

namespace Tools
{
    class State {
    public:
        Entry* m_History;
        Entry* m_Current;
        Entry* m_Latest;

        int m_MaxX;
        int m_MaxY;
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
