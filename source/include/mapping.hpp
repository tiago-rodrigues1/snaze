#ifndef MAPPING_HPP
#define MAPPING_HPP

#include <string>
#include <unordered_map>

#include "common.hpp"

std::unordered_map<char, tile_type_e> char_2_tile {
  {'#', WALL},
  {' ', EMPTY},
  {'V', SNAKEHEAD},
  {'O', SNAKEBODY},
  {'*', FOOD},
  {'.', INVISIBLE_WALL},
  {'&', SPAWN}
};

std::unordered_map<tile_type_e, char> tile_2_char {
  {WALL, '#'},
  {EMPTY, ' '},
  {SNAKEHEAD, 'V'},
  {SNAKEBODY, 'O'},
  {FOOD, '*'},
  {SPAWN, '&'}
};

std::unordered_map<tile_type_e, std::string> tile_2_string {
  {WALL, "█"},
  {FOOD, "🍎"},
  {SNAKEBODY, "●"},
  {SNAKEHEAD, "V"},
  {INVISIBLE_WALL, "."},
  {EMPTY, " "},
  {SPAWN, "𖦹"}
};

#endif