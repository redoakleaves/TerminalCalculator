#include "tools/color.h"

#include "entry.h"

uint32_t current_identifier = 1;

Entry::Entry() {
    identifier = current_identifier++;

    next = nullptr;
    prev = nullptr;
}
Entry::~Entry() {
    if (next)
        delete next;
}

void Entry::set_stylized(std::string& stylized) {
    stylized_content = stylized;
}
void Entry::set_result(std::string& result) {
    this->result = result;
}
void Entry::print_stylized() {
    printw("%s", stylized_content.c_str());

    if (!result.empty()) {
        printw(" = ");
        attron(COLOR_PAIR(COLOR_RESULT));
        printw("%s", result.c_str());
        attroff(COLOR_PAIR(COLOR_RESULT));
    }
}
