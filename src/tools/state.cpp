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
        std::string use_deg_setting = globalconfig.get_value("general.use_deg");
        if (use_deg_setting.empty() || use_deg_setting == "true")
            use_deg = 1;
        else
            use_deg = 0;
    }

    void State::create_new_entry() {
        Entry* new_entry = latest->next = new Entry();
        new_entry->prev = latest;
        current = latest = new_entry;
    }
}
