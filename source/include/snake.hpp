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


class Snake {
private:
  Level* running_level;
  int m_lives;
  TilePos m_head;
  int m_growth_counter = 0;
  std::deque<TilePos> m_body;
  
  public:
  direction_e current_dir;
  Snake(int lives);
  
  static std::unique_ptr<Snake> create_snake(int lives) { return std::make_unique<Snake>(lives); }

  TilePos head() const { return m_head; };
  std::deque<TilePos> body() const { return m_body; };
  TilePos next_head_location(direction_e dir);
  int lives() { return m_lives; };
  void bind_level(Level* level);
  void spawn();
  void die() {
    m_lives -= 1;
  };
  void grow(int amount = 1);
  void step_foward();
  void step_backward();
  TilePos get_next_location();
  bool move_to(direction_e direction);
};

#endif