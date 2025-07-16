#include <fstream>
#include <iostream>
#include <random>

#include "level.hpp"
#include "mapping.hpp"
#include "snake.hpp"
#include "utils.hpp"

Level::Level(std::vector<std::string> b, size_t r, size_t c, TilePos s)
    : board(b), rows(r), cols(c), snake_spawn(s) {}

std::string remove_invalid_chars(const std::string& line,
                                 TilePos& spawn,
                                 const std::string& valids = " #. &") {
  std::string clone{ line };

  for (size_t i{ 0 }; i < clone.size(); ++i) {
    if (clone[i] == tile_2_char[SPAWN]) {
      spawn.col = i;
    }

    if (valids.find(clone[i]) == std::string::npos) {
      clone[i] = ' ';
    }
  }

  return clone;
}

void get_dimensions(const std::string& line, int& rows, int& cols) {
  auto dimensions{ split(line, ' ') };

  if (dimensions.size() != 2) {
    std::cerr << ">>> Dimensions must be: <int> <int>\n";
    exit(EXIT_FAILURE);
  }

  rows = std::stoi(dimensions[0]);
  cols = std::stoi(dimensions[1]);
}

std::vector<Level> Level::level_parser(const std::string& path) {
  std::vector<Level> levels;
  std::ifstream level_file{ path };

  if (path.empty()) {
    std::cerr << ">>> File input missing\n";
    exit(EXIT_FAILURE);
  }

  if (!is_valid_file(path) or !level_file.is_open()) {
    std::cerr << ">>> Sorry, could not read " << path << '\n';
    exit(EXIT_FAILURE);
  }

  int rows{ 0 };
  int cols{ 0 };
  std::vector<std::string> board;
  TilePos spawn_loc = { 0, 0 };

  std::string line;

  while (std::getline(level_file, line)) {
    line = trim(line);

    if (line.size() == 0) {
      continue;
    }

    if (rows == 0 and cols == 0) {
      get_dimensions(line, rows, cols);

      if (rows <= 0 or rows > MAX_GRID_SIZE or cols <= 0 or cols > MAX_GRID_SIZE) {
        std::cerr << ">>> " << path << " has invalid dimensions on level #" << (levels.size() + 1)
                  << ". Must be greater than 0 and less than " << MAX_GRID_SIZE << '\n';
        exit(EXIT_FAILURE);
      }
    } else {
      line = remove_invalid_chars(line, spawn_loc);
      board.push_back(line);

      if (spawn_loc.col == 0) {
        spawn_loc.row += 1;
      }
    }

    if (board.size() == (size_t)rows) {
      Level level(board, rows, cols, spawn_loc);
      levels.push_back(level);

      rows = 0;
      cols = 0;
      board.clear();
    }
  }

  level_file.close();

  return levels;
}

char Level::get_content_at(const TilePos& loc) const { return board[loc.row][loc.col]; }

bool Level::in_board(const TilePos& loc) const {
  return loc.row >= 0 && loc.row < rows && loc.col >= 0 && loc.col < cols;
}

bool Level::is_snake(const TilePos& loc) const {
  return (get_content_at(loc) == 'V' || get_content_at(loc) == 'A');
};

bool Level::is_blocked(const TilePos& loc) const { return get_content_at(loc) == '.'; }

bool Level::is_free(const TilePos& loc) const { return get_content_at(loc) == ' '; }

bool Level::is_wall(const TilePos& loc) const { return get_content_at(loc) == '#'; }

bool Level::is_food(const TilePos& loc) const { return get_content_at(loc) == '*'; }

void Level::print(int lives, int score, int food_to_eat) {
  std::cout << "Lives: ";
  for (size_t i{ 0 }; i < (size_t)lives; ++i) {
    std::cout << "♥";
  }

  std::cout << "| Score: " << score << " | Food eaten: " << food_eaten << " out of " << food_to_eat
            << "\n";

  std::cout << std::string(60, '-') << "\n\n";

  for (const auto& row : board) {
    for (char c : row) {
      tile_type_e tile_type{ char_2_tile[c] };
      std::cout << tile_2_string[tile_type];

      // std::cout << c;
    }

    std::cout << "\n";
  }

  std::cout << '\n' << std::string(60, '-') << '\n';
}

TilePos Level::get_snake_spawn_loc() const { return snake_spawn; };

TilePos Level::get_food_loc() const { return pellet_loc; };

std::vector<TilePos> Level::get_empty_tiles() const {
  std::vector<TilePos> empty_tiles;
  for (size_t i{ 0 }; i < rows; ++i) {
    for (size_t j{ 0 }; j < cols; ++j) {
      if (board[i][j] == ' ') {
        TilePos empty;
        empty.col = j;
        empty.row = i;
        empty_tiles.push_back(empty);
      }
    }
  }

  return empty_tiles;
}

void Level::set_content_at(const TilePos& loc, tile_type_e type) {
  // std::cout << ">>> " << tile_2_char[type] << '\n';
  board[loc.row][loc.col] = tile_2_char[type];
}

void Level::place_pellet() {
  std::vector<TilePos> empty_tiles = get_empty_tiles();

  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_int_distribution<> range(0, empty_tiles.size() - 1);

  int random_tile_idx = range(gen);
  pellet_loc = empty_tiles[random_tile_idx];

  set_content_at(pellet_loc, tile_type_e::FOOD);
}

void Level::remove_food() { 
  std::cout << "Removing food (" << pellet_loc.row << ", " << pellet_loc.col << ")\n";
  set_content_at(pellet_loc, tile_type_e::EMPTY);
}

void Level::place_snake(const Snake* snake, const direction_e& head_dir, bool is_dead) {
  std::cout << ">>>" << dir_labels[head_dir] << '\n';
  set_content_at(snake->head(), dir_snakehead[head_dir]);

  if (!snake->body().empty()) {
    for (TilePos body_part : snake->body()) {
      set_content_at(body_part, tile_type_e::SNAKEBODY);
    }
  }
}

void Level::remove_snake(const Snake* snake) {
  set_content_at(snake->head(), tile_type_e::EMPTY);

  if (!snake->body().empty()) {
    for (TilePos body_part : snake->body()) {
      set_content_at(body_part, tile_type_e::EMPTY);
    }
  }
}

void Level::set_board(std::vector<std::string> m_board) { board = m_board; }
