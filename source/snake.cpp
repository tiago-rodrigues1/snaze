#include "snake.hpp"
#include <iostream>
#include "level.hpp"
#include "player.hpp"

/**
 * Constructs a Snake with a given number of lives.
 * 
 * @param lives Number of lives the snake starts with.
 */
Snake::Snake(int lives) : m_lives(lives) {}

/**
 * Calculates the next head position given a direction.
 * 
 * @param dir Direction to move.
 * @return TilePos of the new head location.
 */
TilePos Snake::next_head_location(direction_e dir) {
  auto [dir_row, dir_col] = dir_map.at(dir);
  size_t new_row{ m_head.row + dir_row };
  size_t new_col{ m_head.col + dir_col };

  return { new_row, new_col };
}

/**
 * Moves the snake in a given direction.
 * 
 * Updates head and body position. Grows if needed. 
 * Returns true if it hits a wall, snake, or block.
 *
 * @param dir Direction to move.
 * @return True if collision occurred.
 */
bool Snake::move_to(direction_e dir) {
  if (dir == INVALID) {
    return true;
  }
  

  TilePos new_head_loc{ next_head_location(dir) };

  m_body.push_front(m_head);
  m_head = new_head_loc;

  if (m_growth_counter > 0) {
    --m_growth_counter;
  } else {
    m_body.pop_back();
  }

  return running_level->is_blocked(new_head_loc) or running_level->is_wall(new_head_loc)
         or running_level->is_snake(new_head_loc);
}

/**
 * Increases the snake's growth counter.
 * 
 * @param amount How many segments to grow.
 */
void Snake::grow(int amount) {
  m_growth_counter += amount;
}

/**
 * Spawns the snake at the initial spawn location.
 */
void Snake::spawn() {
  m_body.clear();

  auto sp = running_level->get_snake_spawn_loc();
  m_head = sp;
  running_level->set_content_at(m_head, SNAKEHEAD);
  current_dir = NONE;
  m_growth_counter = 2;
}
/**
 * Binds the snake to the current level.
 * @param level Pointer to the level where the snake exists.
 */
void Snake::bind_level(Level* level) { running_level = level; }

