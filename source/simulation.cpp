#include <iostream>
#include <stdexcept>

#include "simulation.h"

RunningOpt SnazeSimulation::run_options;
Snake* SnazeSimulation::snake = nullptr;
RandomSPlayer* SnazeSimulation::player = nullptr;
Level* SnazeSimulation::running_level = nullptr;
std::vector<Level> SnazeSimulation::levels;
game_state_e SnazeSimulation::game_state = START;

void SnazeSimulation::usage() {

  std::cout << "Usage: snaze [<options>] <input_level_file>\n"
            << "Game simulation options:\n"
            << "--help/-h Print this help text.\n"
            << "--fps/-f <num> Number of frames (board) presented per second.\n"
            << "--lives/-l <num> Number of lives the snake shall have. Default = 5.\n"
            << "--food/-d <num> Number of food pellets for the entire simulation. Default = 10.\n"
            << "--playertype/-p <type> Type of snake intelligence: random, backtracking. Default = "
               "backtracking\n";
}

bool has_next_argument(int i, int argc) {
  if (i + 1 >= argc) {
    std::cout << "Warning: you need to provide a number or a player type\n";
    return false;
  }
  return true;
}

void assign_if_valid_number(const std::string& str_value,
                            RunningOpt& run_options,
                            int run_options_num) {
  try {
    int num = std::stoi(str_value);

    if (num > 0) {
      switch (run_options_num) {
      case 0:
        run_options.fps = num;
        break;
      case 1:
        run_options.lives = num;
        break;
      case 2:
        run_options.food = num;
        break;
      default:
        break;
      }
    } else {
      std::cout << "Warning: please choose a number bigger than 0\n";
    }
  } catch (const std::exception& e) {
    std::cout << "Warning: invalid number\n";
  }
}

void SnazeSimulation::validate_arguments(int argc, char* argv[], RunningOpt& run_options) {
  bool skip_next{ false };

  for (int i{ 1 }; i < argc; ++i) {
    if (skip_next) {
      skip_next = false;
      continue;
    }

    std::string current_arg{ argv[i] };

    if (current_arg == "--help" || current_arg == "-h") {
      usage();
      exit(0);
    }

    else if ((current_arg == "--fps" || current_arg == "-f") && has_next_argument(i, argc)) {
      assign_if_valid_number(argv[i + 1], run_options, 0);
      skip_next = true;
    }

    else if ((current_arg == "--lives" || current_arg == "-l") && has_next_argument(i, argc)) {
      assign_if_valid_number(argv[i + 1], run_options, 1);
      skip_next = true;
    }

    else if ((current_arg == "--food" || current_arg == "-d") && has_next_argument(i, argc)) {
      assign_if_valid_number(argv[i + 1], run_options, 2);
      skip_next = true;
    }

    else if ((current_arg == "--playertype" || current_arg == "-p") && has_next_argument(i, argc)) {
      std::string next_arg{ argv[i + 1] };
      if (next_arg == "random") {
        run_options.player_type = player_type_e::RANDOM;
      } else if (next_arg == "backtracking") {
        run_options.player_type = player_type_e::BACKTRACKING;
      } else {
        std::cout << "Warning: invalid playertype option\n";
      }
      skip_next = true;
    } else {
      if (current_arg.substr(0, 2) == "--" || current_arg[0] == '-') {
        std::cout << "Warning: invalid option\n";
      } else {
        levels = Level::level_parser(current_arg);
      }
    }
  }
}

void SnazeSimulation::initialize(int argc, char* argv[]) {

  if (argc == 1) {
    usage();
    std::exit(1);
  }

  validate_arguments(argc, argv, run_options);
}

bool test_dir(MoveDir current, MoveDir next, int dir = 0) {
  if (dir == 1) {
    current.turn_right();
  } else if (dir == 2) {
    current.turn_left();
  }
  return (current.dx == next.dx && current.dy == next.dy);
}

void SnazeSimulation::move_snake() {
  auto board = running_level->get_board();
  MoveDir next_move = player->next_move(board);
  MoveDir current_dir = snake->actual_direction;

  if (test_dir(current_dir, next_move)) {
    snake->step_foward();
  } else if (test_dir(current_dir, next_move, 1)) {
    snake->actual_direction.turn_right();
    snake->step_foward();
  } else if (test_dir(current_dir, next_move, 2)) {
    snake->actual_direction.turn_left();
    snake->step_foward();
  }
}

void SnazeSimulation::start(Snake* new_snake, Level* level, RandomSPlayer* new_player) {
  snake = new_snake;
  running_level = level;
  player = new_player;
}

void SnazeSimulation::process_events() {
  switch (game_state) {
  case game_state_e::GET_MAZ:
    opening_message();
    level_header();

    break;
  case game_state_e::RUN:
    /* code */
    break;

  default:
    break;
  }
}

void SnazeSimulation::update(){
  switch (game_state) {
  case game_state_e::GET_MAZ:
    game_state = game_state_e::SHOW_MAZE;
    break;
  case game_state_e::SHOW_MAZE:
    game_state = game_state_e::SOLVE_MAZE;
    break;
  case game_state_e::SOLVE_MAZE:
    game_state = game_state_e::RUN;
    break;
  case game_state_e::CRASH:
    game_state = game_state_e::SHOW_MAZE;
    break;
  default:
    break;
  }
}

void SnazeSimulation::opening_message() {
  std::cout << " ---> Welcome to the classic Snake Game  <---\n"
            << "      copyright DIMAp/UFRN 2017-2025\n"
            << "------------------------------------------------------------\n"
            << " Levels loaded:  " << run_options.input_list.size()
            << " | Snake lives: " << run_options.lives << " | Apples to eat: " << run_options.food
            << "\n"
            << " Clear all levels to win the game. Good luck!!!\n"
            << "------------------------------------------------------------\n"
            << " >>> Press <ENTER> to start the game!\n\n";
}

void SnazeSimulation::print_lives() {
  for (size_t i{ 0 }; i < snake->lives; ++i) {
    std::cout << "♥";
  }
}

void SnazeSimulation::level_header() {
  std::cout << " Lives: ";
  print_lives();
  std::cout << "| Score: " << player->score << " | Food eaten: " << running_level->food_eaten
            << " out of " << run_options.food << "\n";
  std::cout << "------------------------------------------------------------\n";
}

void SnazeSimulation::