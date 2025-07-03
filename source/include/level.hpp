#ifndef LEVEL_HPP
#define LEVEL_HPP

#define MAX_GRID_SIZE 100

#include <string>
#include <vector>

#include "common.hpp"

class Level {
private:
  size_t cols;
  size_t rows;
  std::vector<std::string> board;
  TilePos snake_spawn;
  TilePos pellet_loc;
public:
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
};
#endif