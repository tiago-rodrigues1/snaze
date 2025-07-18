#include <fstream>
#include <iostream>
#include <random>

#include "level.hpp"
#include "mapping.hpp"
#include "snake.hpp"
#include "utils.hpp"
/**
 * Constructs a Level object with the given board configuration.
 * 
 * @param b Board layout as a vector of strings.
 * @param r Number of rows.
 * @param c Number of columns.
 * @param s Snake spawn position.
 */
Level::Level(std::vector<std::string> b, size_t r, size_t c, TilePos s)
    : board(b), rows(r), cols(c), snake_spawn(s) {}

/**
 * Removes invalid characters from a board line.
 * 
 * @param line A line of the board.
 * @param spawn Reference to a TilePos where the spawn point will be updated.
 * @param valids A string of valid characters allowed in the level.
 * @return A sanitized version of the input line containing only valid characters.
 */
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

/**
 * Parses a line to extract the number of rows and columns.
 * 
 * @param line The line containing dimensions in the format "<int> <int>".
 * @param rows Output parameter for number of rows.
 * @param cols Output parameter for number of columns.
 */
void get_dimensions(const std::string& line, int& rows, int& cols) {
  auto dimensions{ split(line, ' ') };

  if (dimensions.size() != 2) {
    std::cerr << ">>> Dimensions must be: <int> <int>\n";
    exit(EXIT_FAILURE);
  }

  rows = std::stoi(dimensions[0]);
  cols = std::stoi(dimensions[1]);
}

/**
 * Parses a level file and loads all defined levels.
 * 
 * @param path Path to the level file.
 * @return A vector of Level objects parsed from the file.
 */
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

      spawn_loc = { 0, 0 };

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
      std::cout << "\n" << rows << "\n" << cols << '\n' << spawn_loc.row << '\n' << spawn_loc.col << '\n';
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

/**
 * Gets the character at a specific board position.
 * 
 * @param loc The position to access.
 * @return The character representing the content at the given position.
 */
char Level::get_content_at(const TilePos& loc) const { return board[loc.row][loc.col]; }

/**
 * Checks if a given position is within the board boundaries.
 * 
 * @param loc Position to check.
 * @return true if the position is valid, false otherwise.
 */
bool Level::in_board(const TilePos& loc) const {
  return loc.row >= 0 && loc.row < rows && loc.col >= 0 && loc.col < cols;
}

/**
 * Checks if a given position contains part of the snake.
 * 
 * @param loc Position to check.
 * @return true if it's the snake's head or body, false otherwise.
 */
bool Level::is_snake(const TilePos& loc) const {
  return (get_content_at(loc) == 'V' || get_content_at(loc) == 'A');
};

/**
 * Checks if a position is marked as blocked.
 * 
 * @param loc Position to check.
 * @return true if it's a blocked tile, false otherwise.
 */
bool Level::is_blocked(const TilePos& loc) const { return get_content_at(loc) == '.'; }

/**
 * Checks if a position is empty.
 * 
 * @param loc Position to check.
 * @return true if the tile is empty, false otherwise.
 */
bool Level::is_free(const TilePos& loc) const { return get_content_at(loc) == ' '; }

/**
 * Checks if a position contains a wall.
 * 
 * @param loc Position to check.
 * @return true if it's a wall tile, false otherwise.
 */
bool Level::is_wall(const TilePos& loc) const { return get_content_at(loc) == '#'; }

/**
 * Checks if a position contains food.
 * 
 * @param loc Position to check.
 * @return true if the tile contains food, false otherwise.
 */
bool Level::is_food(const TilePos& loc) const { return get_content_at(loc) == '*'; }

/**
 * Displays the current game board and player stats to the terminal.
 * 
 * @param lives Number of lives remaining.
 * @param score Current score.
 * @param food_to_eat Total amount of food to be collected.
 */
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
      
    }

    std::cout << "\n";
  }

  std::cout << '\n' << std::string(60, '-') << '\n';
}

/**
 * Returns the snake's spawn location.
 * 
 * @return The TilePos where the snake spawns.
 */
TilePos Level::get_snake_spawn_loc() const { return snake_spawn; };

/**
 * Returns the current location of the food pellet.
 * 
 * @return The TilePos of the food pellet.
 */
TilePos Level::get_food_loc() const { return pellet_loc; };

/**
 * Retrieves all empty tile positions on the board.
 * 
 * @return A vector of TilePos representing free tiles.
 */
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

/**
 * Sets a tile's content at a given position.
 * 
 * @param loc The board position to update.
 * @param type The tile type to set (e.g., wall, food, snake body).
 */
void Level::set_content_at(const TilePos& loc, tile_type_e type) {
  board[loc.row][loc.col] = tile_2_char[type];
}

/**
 * Randomly places a food pellet on an empty tile.
 */
void Level::place_pellet() {
  std::vector<TilePos> empty_tiles = get_empty_tiles();

  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_int_distribution<> range(0, empty_tiles.size() - 1);

  int random_tile_idx = range(gen);
  pellet_loc = empty_tiles[random_tile_idx];

  set_content_at(pellet_loc, tile_type_e::FOOD);
}

/**
 * Removes the food pellet from the board.
 */
void Level::remove_food() { 
  set_content_at(pellet_loc, tile_type_e::EMPTY);
}

/**
 * Places the snake on the board.
 * 
 * @param snake Pointer to the Snake object.
 * @param head_dir Initial direction of the snake's head.
 * @param is_dead Flag for snake state (currently unused).
 */
void Level::place_snake( Snake* snake, const direction_e& head_dir, bool is_dead) {
  snake->current_dir = head_dir;
  set_content_at(snake->head(), dir_snakehead[head_dir]);

  if (!snake->body().empty()) {
    for (TilePos body_part : snake->body()) {
      set_content_at(body_part, tile_type_e::SNAKEBODY);
    }
  }
}

/**
 * Removes the snake's head and body from the board.
 * 
 * @param snake Pointer to the Snake object.
 */
void Level::remove_snake(const Snake* snake) {
  set_content_at(snake->head(), tile_type_e::EMPTY);

  if (!snake->body().empty()) {
    for (TilePos body_part : snake->body()) {
      set_content_at(body_part, tile_type_e::EMPTY);
    }
  }
}
