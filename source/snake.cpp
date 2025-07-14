#include "snake.hpp"
#include "level.hpp"
#include "player.hpp"

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
  next.col += actual_direction.dx;
  next.row += actual_direction.dy;

  return next;
}

void Snake::bind_level(Level* level){
  running_level = level;
}

void Snake::move_to(Dir direction){
  switch (direction)
  {
  case Dir::N:
    actual_direction.dx = 0;
    actual_direction.dy = -1;
    break;
  case Dir::L:
    actual_direction.dx = 1;
    actual_direction.dy = 0;
    break;
  case Dir::O:
    actual_direction.dx = -1;
    actual_direction.dy = 0;
    break;
  case Dir::S:
    actual_direction.dx = 0;
    actual_direction.dy = 1;
    break;
  default:
    break;
  }

  step_foward();
}
