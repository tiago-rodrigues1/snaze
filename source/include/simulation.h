#ifndef SNAZE_HPP
#define SNAZE_HPP

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "snake.hpp"
#include "player.hpp"

enum game_state_e : std::uint8_t { NEUTRAL = 0, START, LOAD_LEVEL, SHOW_LEVEL, START_MAZE, SHOW_MAZE, SOLVE_MAZE, RUN, CRASH, GAME_OVER };

class SnazeSimulation {
private:
  static RunningOpt run_options;
  static std::vector<Level> levels;
  static size_t current_level_idx;
  static std::unique_ptr<Snake> snake;
  static std::unique_ptr<SPlayer> player;
  static game_state_e game_state;

  static void validate_arguments(int argc, char* argv[], RunningOpt& run_options);
  static void start();
  static void load_level();
  static void pass_level();
  static void verify_lives();
  
  public:
  static void fps();
  static void process_events();
  static void update();
  static void render();
  static void initialize(int argc, char* argv[]);
  static void move_snake();
  static bool is_over();
};

#endif