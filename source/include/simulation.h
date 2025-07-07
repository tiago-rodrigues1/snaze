#ifndef SNAZE_HPP
#define SNAZE_HPP

#include <cstdint>
#include <string>
#include <vector>

#include "snake.hpp"
#include "player.hpp"

enum game_state_e : std::uint8_t { START = 0, RUN, CRASH, LEVEL_UP, GAME_OVER };

struct RunningOpt {
  int fps{ 5 };
  int lives{ 5 };
  int food{ 10 };
  player_type_e player_type{ player_type_e::BACKTRACKING };
};

class SnazeSimulation {
private:
static std::vector<Level> levels;
static RunningOpt run_options;
static Snake* snake;
static RandomSPlayer* player;
static Level* running_level;
static game_state_e game_state;
static void usage();
static void validate_arguments(int argc, char* argv[], RunningOpt& run_options);


public:
  static void process_events();
  static void start(Snake* snake, Level* level, RandomSPlayer* player);
  static void update();
  static void initialize(int argc, char* argv[]);
  static void move_snake();
};

#endif