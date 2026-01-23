#include "Color.h"
#include "Tools/Config.h"

#if defined(_WIN32)
#include <curses.h>
#else
#include <ncurses.h>
#endif

#include <string>

namespace Tools {

void Color::InitializeColorDefinitions(const Config& config) {
    use_default_colors();
    start_color();

    std::string config_value;

    config_value = config.GetValue("colors.result");
    init_pair(ColorType::Result, config_value.empty() ? COLOR_GREEN : std::stoi(config_value), -1);

    config_value = config.GetValue("colors.commmand");
    init_pair(ColorType::Command, config_value.empty() ? COLOR_YELLOW : std::stoi(config_value), -1);
}

}
