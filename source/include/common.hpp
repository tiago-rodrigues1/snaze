#ifndef COMMON_HPP
#define COMMON_HPP

#include <cstddef>
#include <string>

/// location inside a board
struct TilePos {
  size_t row;
  size_t col;
};

enum tile_type_e : std::uint8_t {
  EMPTY = 0,
  WALL,
  FOOD, 
  SNAKEHEAD,
  SNAKEBODY,
  INVISIBLE_WALL,
  SPAWN,
};

enum player_type_e : std::uint8_t { RANDOM = 0, BACKTRACKING };
struct RunningOpt {
  int fps{ 5 };
  int lives{ 5 };
  int food{ 10 };
  player_type_e player_type{ player_type_e::BACKTRACKING };
  std::string file_input;
};

#endif
