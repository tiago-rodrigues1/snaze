#include "snake.hpp"

void MoveDir::turn_left() {
  // vira 90 graus com base na direção atual da cobra
  short new_dx = -dy;
  short new_dy = dx;
  dx = new_dx;
  dy = new_dy;
}

void MoveDir::turn_right() {
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