#ifndef _STATE_H_
#define _STATE_H_

#include "entry.h"

class State
{
public:
    Entry* history;
    Entry* current;
    Entry* latest;

    int max_x;
    int max_y;

    int cursor_x;
    int cursor_y;

    State();
    ~State();

    void create_new_entry();
};

#endif
