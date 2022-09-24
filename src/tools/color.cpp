#include <string>

#include "color.h"

void init_color_defs(Config& config) {
    use_default_colors();
    start_color();

    std::string config_value;

    config_value = config.get_value("colors.result");
    init_pair(COLOR_RESULT, config_value.empty() ? COLOR_GREEN : atoi(config_value.c_str()), -1);
}
