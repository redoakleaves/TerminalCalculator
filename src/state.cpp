#include "state.h"

State::State() {
    history = new Entry();
    current = history;
    latest = history;

    cursor_x = 0;
    cursor_y = 0;
}
State::~State() {
    delete history;
}

void State::create_new_entry() {
    Entry* new_entry = latest->next = new Entry();
    new_entry->prev = latest;
    current = latest = new_entry;
}
