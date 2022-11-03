#ifndef _VARS_H_
#define _VARS_H_

#include <string>

#include "entry.h"

/**
 * @brief Searches for variable definition in substring. A valid definition is only stored during the final pass.
 * 
 * @param entry Reference to the current entry.
 * @param substring Reference to the substring.
 * @param final Boolean signaling final pass.
 * 
 * @return 1 on success, 0 otherwise.
*/
int parse_var_def(Entry& entry, std::string& substring, int final = 0);
/**
 * @brief Searches for variable usage within a substring.
 * 
 * @param entry Reference to the current entry.
 * @param substring Reference to the substring.
*/
void parse_var_usage(Entry& entry, std::string& substring);

#endif
