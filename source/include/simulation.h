#ifndef SNAZE_HPP
#define SNAZE_HPP

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "player.hpp"
#include "snake.hpp"

enum game_state_e : std::uint8_t {
  NEUTRAL = 0,
  START,
  LOAD_LEVEL,
  SHOW_LEVEL,
  START_MAZE,
  SHOW_MAZE,
  SOLVE_MAZE,
  RUN,
  EAT_FOOD,
  CRASH,
  GAME_OVER,
  GAME_END,
  GAME_WON
};

class SPlayer;
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
  static Snake& get_snake() { return *snake.get(); }
  static void pass_level();

public:
  static void solve_maze();
  static void execute_directions();
  static void handle_eat();
  static void handle_crash();

  static void verify_lives();

  static void fps();
  static Level* get_current_level() { return &levels[current_level_idx]; }

  static void process_events();
  static void update();
  static void render();
  static void initialize(int argc, char* argv[]);
  static bool is_over();
};

#endif