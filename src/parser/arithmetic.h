#ifndef _ARITHMETIC_H_
#define _ARITHMETIC_H_

#include <string>

#include "tools/entry.h"

/**
 * @brief Parses arithmetic expressions within a substring.
 * 
 * @param entry Reference to the current entry.
 * @param substring Reference to the substring.
*/
void parse_arithmetic(Tools::Entry& entry, std::string& substring);

#endif
