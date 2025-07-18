#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>

#include <chrono>
#include <thread>
#include "player.hpp"
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

void SnazeSimulation::start() {
  snake = Snake::create_snake(run_options.lives);
  player = SPlayer::create_player(run_options.player_type);

  current_level_idx = 0;
  player->bind_snake(snake.get());
}

void SnazeSimulation::load_level() {
  Level* current_level = get_current_level();

  snake->bind_level(current_level);
  player->bind_level(current_level);
}

void SnazeSimulation::pass_level() {
  ++current_level_idx;
  if (current_level_idx >= levels.size()) {
    game_state = game_state_e::GAME_WON;
  } else {
    game_state = LOAD_LEVEL;
  }
}

void SnazeSimulation::solve_maze() {
  auto snake_ptr = snake.get();
  auto level = get_current_level();

  level->place_pellet();
  snake_ptr->spawn();
}

void SnazeSimulation::execute_directions() {
  auto level = get_current_level();
  auto snake_ptr = snake.get();

  if (player && snake_ptr) {
    auto dir = player->next_move();
    level->remove_snake(snake_ptr);

    bool mortal_move{ snake_ptr->move_to(dir) };

    if (!mortal_move) {
      if (level->is_food(snake_ptr->head())) {
        game_state = EAT_FOOD;
      }
    } else {
      game_state = CRASH;
    }

    level->place_snake(snake_ptr, dir);

    // Render dead snake
    if (game_state == CRASH) {
      render();
    }

    if (game_state == CRASH or game_state == EAT_FOOD) {
      process_events();
    }
  } else {
    std::cerr << "Player ou Snake inválidos!\n";
  }
}

void SnazeSimulation::handle_eat() {
  auto snake_ptr = snake.get();
  auto player_ptr = player.get();
  auto level = get_current_level();

  snake_ptr->grow();
  player_ptr->update_score();
  level->update_food_eaten();

  if (level->get_food_eaten() >= run_options.food) {
    game_state = PASS_LEVEL;
  } else {
    level->place_pellet();
    game_state = RUN;
    process_events();
  }
}

void SnazeSimulation::handle_crash() {
  auto snake_ptr = snake.get();
  auto level = get_current_level();

  snake_ptr->die();
  level->remove_snake(snake_ptr);
  level->remove_food();

  if (snake_ptr->lives() <= 0) {
    game_state = GAME_OVER;
  } else {
    game_state = SOLVE_MAZE;
  }

  process_events();
}

void SnazeSimulation::process_events() {
  if (game_state == START) {
    std::cout << "OIII";
    start();
  } else if (game_state == LOAD_LEVEL) {
    std::cout << "OIII";
    load_level();
  } else if (game_state == SOLVE_MAZE) {
    solve_maze();
  } else if (game_state == RUN) {
    execute_directions();
  } else if (game_state == EAT_FOOD) {
    handle_eat();
  } else if (game_state == CRASH) {
    handle_crash();
  } else if (game_state == PASS_LEVEL) {
    pass_level();
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
    game_state = SOLVE_MAZE;
  } else if (game_state == SOLVE_MAZE) {
    game_state = RUN;
  } else if (game_state == EAT_FOOD) {
    game_state = RUN;
  } else if (game_state == CRASH) {
    game_state = SOLVE_MAZE;
  }
}

void SnazeSimulation::render() {
  if (game_state == START) {
    SnazeRender::welcome(levels.size(), run_options.lives, run_options.food);
  } else if (game_state == SHOW_LEVEL || game_state == RUN || game_state == PASS_LEVEL) {
    levels[current_level_idx].print(snake->lives(), player->score(), run_options.food);
  } else if (game_state == RUN) {
    levels[current_level_idx].print(snake->lives(), player->score(), run_options.food);
  } else if (game_state == CRASH) {
    levels[current_level_idx].print(snake->lives(), player->score(), run_options.food);

    if (snake->lives() - 1 > 0) {
      SnazeRender::get_input(">>> Press <ENTER> to try again.");
    }
  } else if (game_state == GAME_OVER) {
    SnazeRender::game_over(player->score());
  } else if (game_state == GAME_WON) {
    SnazeRender::game_won(player->score());
  }
}

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

bool SnazeSimulation::is_over() { return game_state == GAME_OVER || game_state == GAME_WON; }