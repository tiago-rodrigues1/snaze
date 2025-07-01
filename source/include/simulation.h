#ifndef SNAZE_HPP
#define SNAZE_HPP

#include <vector>
#include <string>
#include <cstdint>
#include "snake.hpp"

enum game_state_e : std::uint8_t {
  START = 0,
  RUN,
  CRASH,
  LEVEL_UP,
  GAME_OVER
};

enum player_type_e : std::uint8_t
{
  RANDOM = 0,
  BACKTRACKING,
  HUMAN
};

struct RunningOpt
{
  std::vector<std::string> input_list;
  int fps{5};
  int lives{5};
  int food{10};
  player_type_e player_type{player_type_e::BACKTRACKING};
};

class SnazeSimulation
{
private:
static RunningOpt run_options;
static Snake snake;

static void usage();
static void validate_arguments(int argc, char *argv[], RunningOpt &run_options);

public:
  static void process_events();
  static void update();
  static void initialize(int argc, char *argv[]);
};

#endif