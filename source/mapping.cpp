#include "mapping.hpp"

std::unordered_map<char, tile_type_e> char_2_tile = {
  {'#', WALL},
  {' ', EMPTY},
  {'v', SNAKEHEAD},
  {'^', SNAKEHEAD_NORTH},
  {'>', SNAKEHEAD_EAST},
  {'<', SNAKEHEAD_WEST},
  {'x', SNAKEHEAD_DEAD},
  {'O', SNAKEBODY},
  {'*', FOOD},
  {'.', INVISIBLE_WALL},
  {'&', SPAWN}
};

std::unordered_map<tile_type_e, char> tile_2_char = {
  {WALL, '#'},
  {EMPTY, ' '},
  {SNAKEHEAD, 'v'},
  {SNAKEHEAD_NORTH, '^'},
  {SNAKEHEAD_EAST, '>'},
  {SNAKEHEAD_WEST, '<'},
  {SNAKEHEAD_DEAD, 'x'},
  {SNAKEBODY, 'O'},
  {FOOD, '*'},
  {SPAWN, '&'}
};

std::unordered_map<tile_type_e, std::string> tile_2_string = {
  {WALL, "█"},
  {FOOD, "✿"},
  {SNAKEBODY, "●"},
  {SNAKEHEAD, "v"},
  {SNAKEHEAD_NORTH, "^"},
  {SNAKEHEAD_EAST, ">"},
  {SNAKEHEAD_WEST, "<"},
  {SNAKEHEAD_DEAD, "🕱"},
  {INVISIBLE_WALL, "."},
  {EMPTY, " "},
  {SPAWN, "𖦹"}
};

std::unordered_map<direction_e, std::pair<int, int>> dir_map = {
  {NORTH, {-1, 0}},
  {EAST, {0, 1}},
  {SOUTH, {1, 0}},
  {WEST, {0, -1}},
  {INVALID, {0, 0}}
};

std::unordered_map<direction_e, std::string> dir_labels = {
  {NORTH, "NORTH"},
  {EAST, "EAST"},
  {SOUTH, "SOUTH"},
  {WEST, "WEST"}
};

std::unordered_map<direction_e, tile_type_e> dir_snakehead = {
  {NORTH, SNAKEHEAD_NORTH},
  {EAST, SNAKEHEAD_EAST},
  {SOUTH, SNAKEHEAD},
  {WEST, SNAKEHEAD_WEST},
  {INVALID, SNAKEHEAD_DEAD}
};