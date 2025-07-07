#ifndef LEVEL_HPP
#define LEVEL_HPP

#define MAX_GRID_SIZE 100

#include <string>
#include <vector>

#include "common.hpp"

class Snake;

enum tile_type_e : std::uint8_t {
  EMPTY = 0,
  WALL,
  FOOD, 
  SNAKEHEAD,
  SNAKEBODY
};

class Level {
private:
  std::vector<std::string> board;
  size_t rows;
  size_t cols;
  TilePos snake_spawn;
  TilePos pellet_loc;
public:
  int food_eaten;
  Level(std::vector<std::string> b, size_t r, size_t c);

  static std::vector<Level> level_parser(const std::string& path);

  size_t n_cols() { return cols; };
  size_t n_rows() { return rows; };

  std::vector<std::string> get_board() { return board; };

  // TODO: use mapping
  char get_content_at(const TilePos& loc) const;

  bool in_board(const TilePos& loc) const;
  bool is_blocked(const TilePos& loc) const;
  bool is_free(const TilePos& loc) const;
  bool is_wall(const TilePos& loc) const;
  bool is_snake(const TilePos& loc) const;
  bool is_food(const TilePos& loc) const;
  void print();
  TilePos get_snake_spawn_loc() const;
  void remove_food();
  TilePos get_food_loc() const;
  void place_pellet();
  std::vector<TilePos> get_empty_tiles() const;
  void set_content_at(const TilePos& loc,  tile_type_e type);
  void remove_snake(const Snake& snake);
  void set_board(std::vector<std::string> m_board);
};
#endif