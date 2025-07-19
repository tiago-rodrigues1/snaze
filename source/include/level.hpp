#ifndef LEVEL_HPP
#define LEVEL_HPP

#define MAX_GRID_SIZE 100

#include <string>
#include <vector>

#include "common.hpp"

class Snake;

class Level {
private:
  std::vector<std::string> board;
  size_t rows;
  size_t cols;
  TilePos snake_spawn;
  TilePos pellet_loc;
  int food_eaten;
  bool level_completed;
public:
  Level(std::vector<std::string> b, size_t r, size_t c, TilePos s);

  static std::vector<Level> level_parser(const std::string& path);

  size_t n_cols() { return cols; };
  size_t n_rows() { return rows; };

  std::vector<std::string> get_board() { return board; };

  char get_content_at(const TilePos& loc) const;
  bool in_board(const TilePos& loc) const;
  bool is_blocked(const TilePos& loc) const;
  bool is_free(const TilePos& loc) const;
  bool is_wall(const TilePos& loc) const;
  bool is_snake(const TilePos& loc) const;
  bool is_food(const TilePos& loc) const;
  void print(int lives, int score, int food_to_eat);
  void update_food_eaten(int amount = 1) { food_eaten += amount; };
  TilePos get_snake_spawn_loc() const;
  void remove_food();
  TilePos get_food_loc() const;
  void place_pellet();
  std::vector<TilePos> get_empty_tiles() const;
  void set_content_at(const TilePos& loc,  tile_type_e type);
  void remove_snake(const Snake* snake);
  void place_snake( Snake* snake, const direction_e& head_dir = SOUTH, bool is_dead = false);
  int get_food_eaten() { return food_eaten; };
};
#endif