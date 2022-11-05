#ifndef _FUNC_H_
#define _FUNC_H_

#include <string>
#include <vector>

#include "tools/entry.h"

typedef struct {
    std::string name;
    std::vector<std::string> params;
    std::string expression;
} function_t;

/**
 * @brief Parses function defintion in substring. A valid definition is only stored during the final pass.
 * 
 * @param entry Reference to the current entry.
 * @param substring Reference to the substring.
 * @param final Boolean signaling final pass.
 * 
 * @return 1 on success, 0 otherwise.
*/
int parse_func_def(Tools::Entry& entry, std::string& substring, int final = 0);
/**
 * @brief Searches for and resolves usage of user-defined functions within a substring.
 * 
 * @param entry Reference to the current entry.
 * @param substring Reference to the substring.
*/
void parse_func_usage(Tools::Entry& entry, std::string& substring);
/**
 * @brief Searches for and resolves usage of predefined functions within a substring.
 * 
 * @param entry Reference to the current entry.
 * @param substring Reference to the substring.
*/
void parse_const_func_usage(Tools::Entry& entry, std::string& substring);

#endif
