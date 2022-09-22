#ifndef _ENTRY_H_
#define _ENTRY_H_

#include <string>

extern uint32_t current_identifier;

class Entry {
public:
    uint32_t identifier;

    std::string raw_content;

    Entry* next;
    Entry* prev;

    Entry();
    ~Entry();

    void set_stylized(std::string& stylized);
    void set_result(std::string& result);
    void print_stylized();

private:
    std::string stylized_content;
    std::string result;
};

#endif
