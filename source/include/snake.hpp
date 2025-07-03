#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <cstddef>
#include <cstdint>
#include <vector>

#include "common.hpp"
#include "level.hpp"

struct MoveDir {
  short dx;
  short dy;
  void turn_left();
  void turn_right();

  bool operator==(const MoveDir& other) const {
    return dx == other.dx && dy == other.dy;
  }
};

class Snake {
private:
Level running_level;

public:
MoveDir actual_direction;
std::vector<TilePos> body;
  unsigned short int lives;
  void step_foward();
  void step_backward();
  TilePos get_next_location();
  void bind_level();

};

#endif