#ifndef _ALGEBRA_H_
#define _ALGEBRA_H_

#include <string>

#include "tools/entry.h"

/**
 * @brief Parses algebra expressions within a substring.
 * 
 * @param entry Reference to the current entry.
 * @param substring Reference to the substring.
*/
void parse_algebra(Tools::Entry& entry, std::string& substring);

#endif
