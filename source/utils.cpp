#include <filesystem>

#include "utils.hpp"

/**
 * Trims characters from the beginning of a string.
 *
 * @param s The input string.
 * @param t A C-string containing the characters to trim.
 * @return A new string with leading characters removed.
 */

std::string ltrim(const std::string& s, const char* t) {
  std::string clone{ s };
  clone.erase(0, clone.find_first_not_of(t));
  return clone;
}

/**
 * Trims characters from the end of a string.
 *
 * @param s The input string.
 * @param t A C-string containing the characters to trim.
 * @return A new string with trailing characters removed.
 */
std::string rtrim(const std::string& s, const char* t) {
  std::string clone{ s };
  clone.erase(clone.find_last_not_of(t) + 1);
  return clone;
}

/**
 * Applies ltrim and rtrim in sequence.
 *
 * @param s The input string.
 * @param t A C-string containing the characters to trim.
 * @return A new string with leading and trailing characters removed.
 */
std::string trim(const std::string& s, const char* t) {
  std::string clone{ s };
  clone = ltrim(clone, t);
  clone = rtrim(clone, t);

  return clone;
}

std::vector<std::string> split(const std::string& str, char delimiter) {
  std::vector<std::string> tokens;

  std::istringstream iss;
  iss.str(str);

  std::string token;

  while(std::getline(iss >> std::ws, token, delimiter)) {
    tokens.emplace_back(token);
  }

  return tokens;
}

bool is_valid_file(const std::string& path) {
  return std::filesystem::exists(path) and std::filesystem::is_regular_file(path);
}