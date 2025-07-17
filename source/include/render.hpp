#ifndef SNAZERENDER_HPP
#define SNAZERENDER_HPP

#include "level.hpp"

class SnazeRender {
private:
  SnazeRender() {};

public:
  static void usage();
  static void welcome(int loaded_levels, int lives, int food);
  static void game_over(int score);
  static void game_won(int score);
  static std::string get_input(std::string_view msg);

  static void pass_level();
};

#endif