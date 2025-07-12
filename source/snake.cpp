#include "snake.hpp"
#include "level.hpp"

Snake::Snake(size_t lives) : m_lives(lives) {}

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

void Snake::step_foward() {
  TilePos next = get_next_location();
  body.push_back(next);
  body.erase(body.begin());
}

void Snake::step_backward() {
  // desfazer o movimento
}

TilePos Snake::get_next_location() {
  TilePos next = body.back();
  next.col += actual_direction.dy;
  next.row += actual_direction.dx;

  return next;
}

void Snake::bind_level(Level* level){
  running_level = level;
}

void Snake::set_life(size_t new_life){
  m_lives = new_life;
}