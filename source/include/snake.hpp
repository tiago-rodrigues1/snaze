#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <cstddef>
#include <cstdint>
#include <deque>
#include <memory>

#include "common.hpp"
#include "level.hpp"
#include "mapping.hpp"
#include "simulation.h"

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
  int m_lives;
  TilePos m_head;
  int m_growth_counter = 0;
  std::deque<TilePos> m_body;

public:
  Snake(int lives);
  
  static std::unique_ptr<Snake> create_snake(int lives) {return std::make_unique<Snake>(lives); }

  TilePos head() const { return m_head; };
  std::deque<TilePos> body() const { return m_body; };
  TilePos next_head_location(direction_e dir);
  int lives() { return m_lives; };
  void bind_level(Level* level);
  void spawn();
  void die() {
    m_lives -= 1;
  };
  bool move_to(direction_e dir);
  void grow(int amount = 1);
  void step_foward();
  void step_backward();
};

#endif