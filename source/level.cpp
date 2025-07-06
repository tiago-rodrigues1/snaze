#include <fstream>
#include <iostream>
#include <random>

#include "level.hpp"
#include "utils.hpp"

Level::Level(std::vector<std::string> b, size_t r, size_t c) : board(b), rows(r), cols(c) {}

std::string remove_invalid_chars(const std::string& line, const std::string& valids = " #. &") {
  std::string clone{ line };

  for(size_t i{ 0 }; i < clone.size(); ++i) {
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

  if (!is_valid_file(path) or !level_file.is_open()) {
    std::cerr << ">>> Sorry, could not read " << path << '\n';
    exit(EXIT_FAILURE);
  }

  int rows{ 0 };
  int cols{ 0 };
  std::vector<std::string> board;

  std::string line;

  while (std::getline(level_file, line)) {
    line = trim(line);

    if (line.size() == 0) {
      continue;
    }

    if (rows == 0 and cols == 0) {
      get_dimensions(line, rows, cols);

      if (rows <= 0 or rows > MAX_GRID_SIZE or cols <= 0 or cols > MAX_GRID_SIZE) {
        std::cerr << ">>> " << path << " has invalid dimensions on level #" << (levels.size() + 1) << ". Must be greater than 0 and less than " << MAX_GRID_SIZE << '\n';
        exit(EXIT_FAILURE);
      }
    } else {
      line = remove_invalid_chars(line);
      board.push_back(line)
    }

    if (board.size() == (size_t)rows) {
      Level level(board, rows, cols);
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

bool Level::is_snake(const TilePos& loc) const {return (get_content_at(loc) == 'V' || get_content_at(loc) == 'A');};

bool Level::is_blocked(const TilePos& loc) const { return get_content_at(loc) == '.'; }

bool Level::is_free(const TilePos& loc) const { return get_content_at(loc) == ' '; }

bool Level::is_wall(const TilePos& loc) const { return get_content_at(loc) == '#'; }

void Level::print() {
  for (const std::string& line : board) {
    std::cout << line << '\n';
  }
}

bool Level::is_food(const TilePos& loc) const { return get_content_at(loc) == '*'; }

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
  switch (type) {
  case tile_type_e::EMPTY:
    board[loc.row][loc.col] = ' ';
    break;
  case tile_type_e::WALL:
    board[loc.row][loc.col] = '#';
    break;
  case tile_type_e::FOOD:
    board[loc.row][loc.col] = '*';
    break;
  case tile_type_e::SNAKEHEAD:
    board[loc.row][loc.col] = 'A';
    break;
  case tile_type_e::SNAKEBODY:
    board[loc.row][loc.col] = 'V';
    break;
  default:
    break;
  }
}

void Level::place_pellet() {
  std::vector<TilePos> empty_tiles = get_empty_tiles();

  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_int_distribution<> range(0, empty_tiles.size() - 1);

  int random_tile_idx = range(gen);

  pellet_loc = empty_tiles[random_tile_idx];
  set_content_at(empty_tiles[random_tile_idx], tile_type_e::FOOD);
}

void Level::remove_food() { set_content_at(pellet_loc, tile_type_e::EMPTY); }

void Level::remove_snake(const Snake& snake){
  for(TilePos body_part : snake.body){
    set_content_at(body_part, tile_type_e::EMPTY);
  }
}

void Level::set_board(std::vector<std::string> m_board){
  board = m_board;
}
