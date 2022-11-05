#include <string>

#include "global.h"
#include "state.h"

namespace Tools
{
    State::State() {
        history = new Entry();
        current = history;
        latest = history;

        cursor_x = 0;
        cursor_y = 0;

        use_deg = 0;
    }
    State::~State() {
        delete history;
    }

    void State::init() {
    }

    void State::create_new_entry() {
        Entry* new_entry = latest->next = new Entry();
        new_entry->prev = latest;
        current = latest = new_entry;
    }
}
