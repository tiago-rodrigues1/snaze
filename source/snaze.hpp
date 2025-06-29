#ifndef SNAZE_HPP
#define SNAZE_HPP

#include <vector>
#include <string>
#include <cstdint>

enum player_type_e : std::uint8_t
{
  RANDOM = 0,
  BACKTRACKING
};

struct RunningOpt
{
  std::vector<std::string> input_list;
  int fps;
  int lives{5};
  int food{10};
  player_type_e player_type{player_type_e::BACKTRACKING};
};

class SnazeSimulation
{
private:
static void usage();
static void validate_arguments(int argc, char *argv[], RunningOpt &run_options);
static RunningOpt run_options;
public:
  static void initialize(int argc, char *argv[]);
};

#endif