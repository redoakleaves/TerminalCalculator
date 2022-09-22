#include "color.h"

void init_color_defs() {
    use_default_colors();
    start_color();
    init_pair(COLOR_RESULT, COLOR_GREEN, -1);
}
