#ifndef _VARS_H_
#define _VARS_H_

#include "entry.h"

int parse_var_def(Entry& entry, std::string& substring, int final = 0);
void parse_var_usage(Entry& entry, std::string& substring);

#endif
