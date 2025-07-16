#include "snake.hpp"
#include <iostream>
#include "level.hpp"

Snake::Snake(int lives) : m_lives(lives) {}

void MoveDir::turn_right() {
  // vira 90 graus com base na direção atual da cobra
  short new_dx = -dy;
  short new_dy = dx;
  dx = new_dx;
  dy = new_dy;
}

void MoveDir::turn_left() {
  short new_dx = dy;
  short new_dy = -dx;
  dx = new_dx;
  dy = new_dy;
}

// void Snake::step_foward() {
//   TilePos next = get_next_location();
//   body.push_back(next);
//   body.erase(body.begin());
// }

void Snake::step_backward() {
  // desfazer o movimento
}

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

  if (running_level->is_food(new_head_loc)) {
    std::cout << "FOOD\n";
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

  std::cout << "SPAWN ROW = " << sp.row << " SPWAN COL = " << sp.col << '\n';

  m_head = sp;
  running_level->set_content_at(m_head, SNAKEHEAD);

  m_growth_counter = 2;
}

void Snake::bind_level(Level* level) { running_level = level; }