#ifndef SNAZE_HPP
#define SNAZE_HPP

#include <cstdint>
#include <string>
#include <vector>
#include "snake.hpp"
#include "player.hpp"

enum game_state_e : std::uint8_t { GET_MAZE = 0, SHOW_MAZE, SOLVE_MAZE, RUN, CRASH, GAME_OVER };


struct RunningOpt {
  std::vector<std::string> input_list;
  int fps{ 5 };
  int lives{ 5 };
  int food{ 10 };
  player_type_e player_type{ player_type_e::BACKTRACKING };
};

class SnazeSimulation {
private:
static RunningOpt run_options;
static Snake* snake;
static RandomSPlayer* player;
static Level* running_level;
static game_state_e game_state;
static void usage();
static void validate_arguments(int argc, char* argv[], RunningOpt& run_options);


public:
  static void print_lives();
  static void level_header();
  static void opening_message();
  static void process_events();
  static void start(Snake* snake, Level* level, RandomSPlayer* player);
  static void update();
  static void initialize(int argc, char* argv[]);
  static void move_snake();
  static void print_level();
};

#endif