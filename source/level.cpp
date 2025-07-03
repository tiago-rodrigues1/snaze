#include <fstream>
#include <iostream>

#include "level.hpp"
#include "utils.hpp"

std::vector<Level> Level::level_parser(const std::string& path) {
  std::ifstream level_file{ path };

  if (!is_valid_file(path) or !level_file.is_open()) {
    std::cerr << ">>> Sorry, could not read " << path << '\n';
    exit(EXIT_FAILURE);
  }

  std::string line;
  std::vector<Level> levels;
  bool dimensions_readed{ false };
  int level_count{ 1 };

  while (std::getline(level_file, line)) {
    Level level;
    line = trim(line);

    if (line.size() == 0) {
      continue;
    }

    int rows{ 0 };
    int cols{ 0 };

    if (!dimensions_readed) {
      auto dimensions{ split(line, ' ') };

      if (dimensions.size() != 2) {
        std::cerr << ">>> Dimensions must be: <int> <int>\n";
        exit(EXIT_FAILURE);
      }

      rows = std::stoi(dimensions[0]);
      cols = std::stoi(dimensions[1]);

      if (rows <= 0 or rows > MAX_GRID_SIZE or cols <= 0 or cols > MAX_GRID_SIZE) {
        std::cerr << ">>> " << path << " has invalid dimensions on level #" << level_count << ". Must be greater than 0 and less than " << MAX_GRID_SIZE << '\n';
        exit(EXIT_FAILURE);
      }

      dimensions_readed = true;
      ++level_count;
    } else {
      
    }
  }

  level_file.close();

  return levels;
}

char Level::get_content_at(const TilePos& loc) const { return board[loc.row][loc.col]; }

bool Level::in_board(const TilePos& loc) const {
  return loc.row >= 0 && loc.row < rows && loc.col >= 0 && loc.col < cols;
}

bool Level::is_blocked(const TilePos& loc) const { return get_content_at(loc) == '.'; }

bool Level::is_free(const TilePos& loc) const { return get_content_at(loc) == ' '; }

bool Level::is_wall(const TilePos& loc) const { return get_content_at(loc) == '#'; }

bool Level::is_food(const TilePos& loc) const { return get_content_at(loc) == '🍎'; }
