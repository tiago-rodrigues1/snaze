#include "snake.hpp"
#include <iostream>
#include "level.hpp"
#include "player.hpp"

Snake::Snake(int lives) : m_lives(lives) {}

TilePos Snake::next_head_location(direction_e dir) {
  auto [dir_row, dir_col] = dir_map.at(dir);
  size_t new_row{ m_head.row + dir_row };
  size_t new_col{ m_head.col + dir_col };

  return { new_row, new_col };
}

bool Snake::move_to(direction_e dir) {
  if (dir == INVALID) {
    return true;
  }
  

  TilePos new_head_loc{ next_head_location(dir) };

  m_body.push_front(m_head);
  m_head = new_head_loc;

  if (m_growth_counter > 0) {
    --m_growth_counter;
  } else {
    m_body.pop_back();
  }

  return running_level->is_blocked(new_head_loc) or running_level->is_wall(new_head_loc)
         or running_level->is_snake(new_head_loc);
}

void Snake::grow(int amount) {
  m_growth_counter += amount;
}

void Snake::spawn() {
  m_body.clear();

  auto sp = running_level->get_snake_spawn_loc();

  m_head = sp;
  running_level->set_content_at(m_head, SNAKEHEAD);
  current_dir = NONE;
  m_growth_counter = 2;
}

void Snake::bind_level(Level* level) { running_level = level; }

