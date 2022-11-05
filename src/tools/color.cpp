#include <string>

#if defined(_WIN32)
#include <curses.h>
#else
#include <ncurses.h>
#endif

#include "tools/config.h"
#include "color.h"

namespace Tools
{
    void init_color_defs(Config& config) {
        use_default_colors();
        start_color();

        std::string config_value;

        config_value = config.get_value("colors.result");
        init_pair(COLOR_RESULT, config_value.empty() ? COLOR_GREEN : std::stoi(config_value), -1);

        config_value = config.get_value("colors.commmand");
        init_pair(COLOR_COMMAND, config_value.empty() ? COLOR_YELLOW : std::stoi(config_value), -1);
    }
}
