#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>

#include "render.hpp"
#include "simulation.h"

RunningOpt SnazeSimulation::run_options;
std::unique_ptr<Snake> SnazeSimulation::snake = nullptr;
std::unique_ptr<SPlayer> SnazeSimulation::player = nullptr;
std::vector<Level> SnazeSimulation::levels;
game_state_e SnazeSimulation::game_state = NEUTRAL;
size_t SnazeSimulation::current_level_idx = 0;

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
      SnazeRender::usage();
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
        run_options.file_input = current_arg;
      }
    }
  }
}

void SnazeSimulation::initialize(int argc, char* argv[]) {

  if (argc == 1) {
    SnazeRender::usage();
    std::exit(1);
  }

  validate_arguments(argc, argv, run_options);
  levels = Level::level_parser(run_options.file_input);
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
  auto board = levels[current_level_idx].get_board();
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

void SnazeSimulation::start() {
  snake = Snake::create_snake(run_options.lives);
  player = SPlayer::create_player(run_options.player_type);

  current_level_idx = 0;
  player->bind_snake(snake.get());
}

void SnazeSimulation::load_level() {
  snake->bind_level(&levels[current_level_idx]);
  player->bind_level(&levels[current_level_idx]);
}

void SnazeSimulation::pass_level() {
  ++current_level_idx;
  if (current_level_idx >= levels.size()) {
    game_state = game_state_e::GAME_OVER;
    return;
  }
  load_level();
}

void SnazeSimulation::verify_lives() {
  size_t current_life = snake->lives();
  if (current_life == 0) {
    pass_level();
    game_state = game_state_e::SHOW_MAZE;
  } else {
    snake->set_life(--current_life);
  }
}

void SnazeSimulation::process_events() {
  if (game_state == START) {
    start();
  } else if (game_state == LOAD_LEVEL) {
    levels[current_level_idx].set_snake(*snake);
    load_level();
  } else if (game_state == game_state_e::SOLVE_MAZE){
    
  }
  else if (game_state == game_state_e::RUN) {
    move_snake();
  }
}

void SnazeSimulation::update() {
  if (game_state == NEUTRAL) {
    game_state = START;
  } else if (game_state == START) {
    game_state = LOAD_LEVEL;
  } else if (game_state == LOAD_LEVEL) {
    game_state = SHOW_LEVEL;
  } else if (game_state == SHOW_LEVEL) {
    game_state == SOLVE_MAZE;
  } else if (game_state == SOLVE_MAZE) {
    game_state == RUN;
  }
}

void SnazeSimulation::render() {
  if (game_state == START) {
    SnazeRender::welcome(levels.size(), run_options.lives, run_options.food);
  } else if (game_state == SHOW_LEVEL) {
    levels[current_level_idx].print(snake->lives(), player->score(), run_options.food);
  } else if (game_state == RUN) {
    levels[current_level_idx].print(snake->lives(), player->score(), run_options.food);
  }
}

bool SnazeSimulation::is_over() { return game_state == SHOW_LEVEL; }

void SnazeSimulation::fps() {
  using clock = std::chrono::steady_clock;
  const int fps = run_options.fps;

  const std::chrono::milliseconds frame_duration(1000 / fps);

  static auto frame_end = clock::now();

  auto now = clock::now();
  auto work_time = std::chrono::duration_cast<std::chrono::milliseconds>(now - frame_end);

  if (work_time < frame_duration) {
    std::this_thread::sleep_for(frame_duration - work_time);
  }
  frame_end = clock::now();
}