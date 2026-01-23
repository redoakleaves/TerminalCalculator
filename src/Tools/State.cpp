#include "State.h"
#include "global.h"

#include <iterator>
#include <string>

namespace Tools {

State::State() {
    history_m.push_back(Entry());
    activeEntry_m = std::prev(history_m.end());

    window_mp = nullptr;

    cursorX_m = 0;
    cursorY_m = 0;

    useDeg_m = false;
}

void State::Init() {
    const std::string useDegSetting = globalConfig.GetValue("general.use_deg");
    if (useDegSetting.empty() || useDegSetting == "true") {
        useDeg_m = true;
    } else {
        useDeg_m = false;
    }
}

void State::CreateNewEntry() {
    history_m.push_back(Entry());
    JumpToLatestEntry();
}

Entry& State::GetActiveEntry() const {
    return *activeEntry_m;
}
Entry& State::GetLatestEntry() const {
    return history_m.back();
}

void State::JumpToLatestEntry() {
    activeEntry_m = std::prev(history_m.end());
}

}
