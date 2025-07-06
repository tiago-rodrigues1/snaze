#include "player.hpp"
#include <random>


MoveDir RandomSPlayer::next_move(std::vector<std::string>& board) {
  MoveDir actual_dir = snake->actual_direction;
  TilePos snake_head = snake->body.back();
  std::vector<MoveDir> possible_moves;
  MoveDir next_move;

  size_t x = snake_head.col;
  size_t y = snake_head.row;

  if (x + 1 < board[0].size() && board[y][x + 1] != '#' && board[y][x + 1] != '.') {
    possible_moves.push_back({ 1, 0 });
  }

  if (x > 0 && board[y][x - 1] != '#' && board[y][x - 1] != '.') {
    possible_moves.push_back({ -1, 0 });
  }

  if (y + 1 < board.size() && board[y + 1][x] != '#' && board[y + 1][x] != '.') {
    possible_moves.push_back({ 0, -1 });
  }

  if (y > 0 && board[y - 1][x] != '#' && board[y - 1][x] != '.') {
    possible_moves.push_back({ 0, 1 });
  }

  if (!possible_moves.empty()) {
    if (possible_moves.size() == 1) {
      next_move = possible_moves[0];
    } else {
      std::random_device rd;
      std::mt19937 gen(rd());

      std::uniform_int_distribution<> range(0, possible_moves.size() - 1);

      int random_tile_idx = range(gen);

      next_move = possible_moves[random_tile_idx];
    }
  } else{
    next_move = actual_dir;
  }

  return next_move;
}

void RandomSPlayer::bind_snake(Snake* s) { snake = s; }
