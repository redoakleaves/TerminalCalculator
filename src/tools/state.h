#ifndef _STATE_H_
#define _STATE_H_

#include "entry.h"

namespace Tools
{
    class State {
    public:
        Entry* history;
        Entry* current;
        Entry* latest;

        int max_x;
        int max_y;
        int cursor_x;
        int cursor_y;

        int use_deg;

        State();
        ~State();

        void init();

        void create_new_entry();
    };
}

#endif
