#include "player.hpp"
#include <cstdint>
#include <queue>
#include <random>
#include <set>

#include <iostream>
#include "mapping.hpp"
#include "player.hpp"
#include "simulation.h"

/**
 * @struct PathUnit
 *
 * Stores the current state of the snake, the path taken to reach this state,
 * and a method to generate a unique key for visited state tracking.
 */
struct PathUnit {
  Snake snake;
  std::deque<direction_e> path;
  direction_e last_dir;
  std::string get_key();
};

/**
 * Factory method to create a new player based on the type.
 * 
 * @param player_type The desired type of player.
 * @return A unique pointer to the created player instance.
 */
std::unique_ptr<SPlayer> SPlayer::create_player(player_type_e player_type) {
  if (player_type == player_type_e::BACKTRACKING) {
    return std::make_unique<BFSPlayer>();
  } else if (player_type == player_type_e::RANDOM) {
    return std::make_unique<RandomSPlayer>();
  } else {
    return nullptr;
  }
}

/**
 * Returns a list of valid movement directions from a given tile.
 * 
 * A direction is valid if the target tile is within the board and either free or contains food.
 * 
 * @param level Pointer to the current level.
 * @param loc Current position to evaluate.
 * @return A vector of valid directions.
 */
std::vector<direction_e> get_valid_directions(const Level* level, const TilePos& loc) {
  std::vector<direction_e> result;

  for (const direction_e& dir : { NORTH, EAST, SOUTH, WEST }) {
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

/**
 * Gets a valid random direction for the snake to move.
 * 
 * @return A valid direction or INVALID if none are possible.
 */
direction_e SPlayer::get_random_dir() {
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

    return possible_moves[random_tile_idx];
  }

  return INVALID;
}

/**
 * Returns the next move for the random player.
 * 
 * @return A random valid direction.
 */
direction_e RandomSPlayer::next_move() {
  return get_random_dir();
}

/**
 * Binds the snake instance to the player.
 * 
 * @param s Pointer to the snake.
 */
void SPlayer::bind_snake(Snake* s) { snake = s; }

/**
 * Binds the level instance to the player.
 * 
 * @param l Pointer to the level.
 */
void SPlayer::bind_level(Level* l) { current_level = l; };

/**
 * Returns the type of the player.
 * 
 * @return player_type_e::RANDOM
 */
player_type_e RandomSPlayer::type() const { return player_type_e::RANDOM; }

/**
 * Returns the type of the player.
 * 
 * @return player_type_e::BACKTRACKING
 */
player_type_e BFSPlayer::type() const { return player_type_e::BACKTRACKING; };

/**
 * Converts a direction enum to a string label.
 * 
 * @param dir Direction to convert.
 * @return String representation of the direction.
 */
std::string dir_to_str(direction_e dir) {
  if (dir == direction_e::EAST) {
    return "E";
  } else if (dir == direction_e::WEST) {
    return "W";
  } else if (dir == direction_e::NORTH) {
    return "N";
  } else if (dir == direction_e::SOUTH) {
    return "S";
  } else if (dir == direction_e::NONE) {
    return "SP";
}}

/**
 * Returns a unique string representing the snake's current state.
 *
 * Includes direction, head, and body positions. Used to detect repeated states in BFS.
 *
 * @return Unique state key as a string.
 */
std::string PathUnit::get_key() {
  std::string key;
  key.append(dir_to_str(snake.current_dir));

  key.append(",(");
    key.append(std::to_string(snake.head().row));
    key.append(",");
    key.append(std::to_string(snake.head().col));
    key.append(")");

  for (auto body_part : snake.body()) {
    key.append(",(");
    key.append(std::to_string(body_part.row));
    key.append(",");
    key.append(std::to_string(body_part.col));
    key.append(")");
  }

  return key;
}

/**
 * Computes the position resulting from moving in a given direction.
 * 
 * @param snake_head Current position of the snake's head.
 * @param direction Direction to move.
 * @return New TilePos after applying the direction.
 */
TilePos moveto(TilePos snake_head, direction_e direction) {
  switch (direction) {
  case direction_e::NORTH:
    return TilePos{ snake_head.row - 1, snake_head.col };
    break;
  case direction_e::SOUTH:
    return TilePos{ snake_head.row + 1, snake_head.col };
    break;
  case direction_e::EAST:
    return TilePos{ snake_head.row, snake_head.col + 1 };
    break;
  case direction_e::WEST:
    return TilePos{ snake_head.row, snake_head.col - 1 };
    break;
  default:
    break;
  }
}

/**
 * Executes the BFS algorithm to find a path to the food.
 * 
 * The function simulates possible snake movements from the current state and
 * returns a path that leads to the food, if one exists.
 * 
 * @return A deque of directions that lead the snake to the food.
 */
std::deque<direction_e> BFSPlayer::path_finder() {
  std::queue<PathUnit> places_to_visit;
  std::set<std::string> inspected;
  auto snakebfs = SnazeSimulation::get_snake();
  current_level->remove_snake(&snakebfs);
  PathUnit spawn_loc{ snakebfs, {} };
  places_to_visit.push(spawn_loc);
  inspected.insert(spawn_loc.get_key());  
  while (not places_to_visit.empty()) {
    auto current_loc = places_to_visit.front();
    places_to_visit.pop();
    Snake& csnake = current_loc.snake;  
    current_level->place_snake(&csnake);
    for (direction_e dir : { NORTH, EAST, SOUTH, WEST }) {
      auto destination = moveto(csnake.head(), dir);  
      if (current_level->is_food(destination)) {     
        current_loc.path.push_back(dir);              
        current_level->remove_snake(&csnake);
        return current_loc.path;                    
      }
     
      if (current_level->is_free(destination)) {
        PathUnit new_loc{ current_loc };  
        new_loc.snake.move_to(dir);
        new_loc.path.push_back(dir);  
        if (inspected.count(new_loc.get_key()) == 0) {
          places_to_visit.push(new_loc);        
          inspected.insert(new_loc.get_key());  
        }
      }
    }
    current_level->remove_snake(&csnake);
  }
  return {};  
}

/**
 * Returns the next move for the BFS player.
 * 
 * If a valid path to food exists, follows it step-by-step.
 * Otherwise, falls back to random movement.
 * 
 * @return The next direction to move.
 */
direction_e BFSPlayer::next_move() {

  if (solution.empty()) {
    if (!random_mode) {
      solution = path_finder();
    }

    // If new solution is empty
    if (solution.empty()) {
      random_mode = true;
    }
  }

  if (random_mode) {
    return get_random_dir();
  } else {
    direction_e next_dir = solution.front();
    solution.pop_front();
    return next_dir;
  }
}
