#include <iostream>

#include "render.hpp"

/**
 * Prints the usage instructions for the Snaze game.
 */
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

/**
 * Displays the welcome screen with game details.
 * 
 * @param loaded_levels Number of levels loaded.
 * @param lives Number of lives the snake starts with.
 * @param food Number of food pellets to eat.
 */
void SnazeRender::welcome(int loaded_levels, int lives, int food) {
  std::cout << " ---> Welcome to the classic Snake Game  <---\n"
            << "      copyright DIMAp/UFRN 2017-2025\n"
            << "------------------------------------------------------------\n"
            << " Levels loaded:  " << loaded_levels
            << " | Snake lives: " << lives << " | Apples to eat: " << food
            << "\n"
            << " Clear all levels to win the game. Good luck!!!\n"
            << "------------------------------------------------------------\n";

  get_input(" >>> Press <ENTER> to start the game!\n");
}

/**
 * Displays the Game Over screen with the final score.
 * 
 * @param score The player's final score.
 */
void SnazeRender::game_over(int score) {
  std::cout << "\n";
  std::cout << "       ✖✖✖   G A M E   O V E R   ✖✖✖\n";
  std::cout << "\n";
  std::cout << "          Your final score was:\n";
  std::cout << "                " << score << "\n";
  std::cout << "\n";
  std::cout << "        Better luck next time!\n";
  std::cout << "\n";
}

/**
 * Displays the Game Won screen with the final score.
 * 
 * @param score The player's final score.
 */
void SnazeRender::game_won(int score) {
  std::cout << "\n";
  std::cout << "       ✔✔✔   G A M E   W O N   ✔✔✔\n";
  std::cout << "\n";
  std::cout << "          Your final score was:\n";
  std::cout << "                " << score << "\n";
  std::cout << "\n";
  std::cout << "                Good job!\n";
  std::cout << "\n";
}


/**
 * @Prints a message and waits for user input.
 * 
 * @param msg Message to display to the player.
 * @return The user input as a string.
 */
std::string SnazeRender::get_input(std::string_view msg) { 
  std::cout << msg << '\n';
  std::string input; 
  std::getline(std::cin, input);

  return input;
}
