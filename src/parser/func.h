#ifndef _FUNC_H_
#define _FUNC_H_

#include <vector>

#include "entry.h"

typedef struct {
    std::string name;
    std::vector<std::string> params;
    std::string expression;
} function_t;

int parse_func_def(Entry& entry, std::string& substring, int final = 0);
void parse_func_usage(Entry& entry, std::string& substring);

#endif
