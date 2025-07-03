#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>

std::string ltrim(const std::string& s, const char* t = " \t\n\r\f\v");
std::string rtrim(const std::string& s, const char* t = " \t\n\r\f\v");
std::string trim(const std::string& s, const char* t = " \t\n\r\f\v");
std::vector<std::string> split(const std::string& str, char delimiter);
bool is_valid_file(const std::string& path);

#endif