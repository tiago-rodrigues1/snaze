#include <random>

#include "mapping.hpp"
#include "player.hpp"
#include "simulation.h"
#include <iostream>

std::unique_ptr<SPlayer> SPlayer::create_player(player_type_e player_type) {
  if (player_type == player_type_e::BACKTRACKING) {
    return nullptr;
  } else if (player_type == player_type_e::RANDOM) {
    return std::make_unique<RandomSPlayer>();
  } else {
    return nullptr;
  }
}

std::vector<direction_e> get_valid_directions(const Level* level, const TilePos& loc) {
  std::vector<direction_e> result;

  for (const direction_e& dir : {NORTH, EAST, SOUTH, WEST}) {
    TilePos neighbor;
    auto [dir_row, dir_col] = dir_map[dir];

    neighbor.row = loc.row + dir_row;
    neighbor.col = loc.col + dir_col;

    if (level->in_board(neighbor) && (level->is_free(neighbor) || level->is_food(neighbor))) {
      result.emplace_back(dir);
    }
  }

  return result;
}

direction_e RandomSPlayer::next_move() {
  TilePos snake_head{ snake->head() };
  auto possible_moves{ get_valid_directions(current_level, snake_head) };

  if (!possible_moves.empty()) {
    if (possible_moves.size() == 1) {
      return possible_moves[0];
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> range(0, possible_moves.size() - 1);
    int random_tile_idx = range(gen);

    switch (possible_moves[random_tile_idx]) {
      case NORTH: std::cout << "> Moving to NORTH\n"; break;
      case EAST: std::cout << "> Moving to EAST\n"; break;
      case WEST: std::cout << "> Moving to WEST\n"; break;
      case SOUTH: std::cout << "> Moving to SOUTH\n"; break;
    }

    return possible_moves[random_tile_idx];
  }

  return INVALID;
}

void RandomSPlayer::bind_snake(Snake* s) { snake = s; }

player_type_e RandomSPlayer::type() const { return player_type_e::RANDOM; }

void RandomSPlayer::bind_level(Level* l) { current_level = l; };
