#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <cstddef>
#include <cstdint>
#include <memory>
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
  Level* running_level;
  size_t m_lives;

  
public:
  Snake(size_t lives);
  
  static std::unique_ptr<Snake> create_snake(size_t lives) {
    return std::make_unique<Snake>(lives);
  }

  MoveDir actual_direction;
  std::vector<TilePos> body;
  void step_foward();
  void step_backward();
  TilePos get_next_location();
  void bind_level(Level* level);
  size_t lives() { return m_lives; };
  void set_life(size_t new_life);
};

#endif