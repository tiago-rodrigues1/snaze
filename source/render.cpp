#include <iostream>

#include "render.hpp"

void SnazeRender::usage() {

  std::cout << "Usage: snaze [<options>] <input_level_file>\n"
            << "Game simulation options:\n"
            << "--help/-h Print this help text.\n"
            << "--fps/-f <num> Number of frames (board) presented per second.\n"
            << "--lives/-l <num> Number of lives the snake shall have. Default = 5.\n"
            << "--food/-d <num> Number of food pellets for the entire simulation. Default = 10.\n"
            << "--playertype/-p <type> Type of snake intelligence: random, backtracking. Default = "
               "backtracking\n";
}

void SnazeRender::welcome(int loaded_levels, int lives, int food) {
  std::cout << " ---> Welcome to the classic Snake Game  <---\n"
            << "      copyright DIMAp/UFRN 2017-2025\n"
            << "------------------------------------------------------------\n"
            << " Levels loaded:  " << loaded_levels
            << " | Snake lives: " << lives << " | Apples to eat: " << food
            << "\n"
            << " Clear all levels to win the game. Good luck!!!\n"
            << "------------------------------------------------------------\n"
            << " >>> Press <ENTER> to start the game!\n\n";
  std::cin.get();
}