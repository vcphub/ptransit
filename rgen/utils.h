#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

// time conversion related functions
std::string time_mins_to_hhmm(int total_mins);
int time_string_to_mins(std::string input);

// string related utility functions
std::string remove_alpha(std::string s);
bool string_compare(std::string& s1, std::string& s2);

#endif
