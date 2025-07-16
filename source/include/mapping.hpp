#ifndef MAPPING_HPP
#define MAPPING_HPP

#include <string>
#include <unordered_map>

#include "common.hpp"

extern std::unordered_map<char, tile_type_e> char_2_tile;
extern std::unordered_map<tile_type_e, char> tile_2_char;
extern std::unordered_map<tile_type_e, std::string> tile_2_string;
extern std::unordered_map<direction_e, std::pair<int, int>> dir_map;
extern std::unordered_map<direction_e, std::string> dir_labels;
extern std::unordered_map<direction_e, tile_type_e> dir_snakehead;

#endif