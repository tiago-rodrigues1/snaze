#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <cstddef>
#include <cstdint>
#include <vector>

#include "common.hpp"

struct MoveDir {
  short dx;
  short dy;
  void turn_left();
  void turn_right();
};

class Snake {
private:
  std::vector<TilePos> body;
  // SnakeLevel running_level
  MoveDir actual_direction;

public:
  unsigned short int lives;
  void step_foward();
  void step_backward();
  TilePos get_next_location();
  void bind_level();
};

#endif