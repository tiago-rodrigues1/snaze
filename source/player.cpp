#include "player.hpp"
#include <cstdint>
#include <queue>
#include <random>
#include <set>

#include <iostream>
#include "mapping.hpp"
#include "player.hpp"
#include "simulation.h"

struct PathUnit {
  Snake snake;
  std::deque<direction_e> path;
  direction_e last_dir;
  std::string get_key();
};

std::unique_ptr<SPlayer> SPlayer::create_player(player_type_e player_type) {
  if (player_type == player_type_e::BACKTRACKING) {
    return std::make_unique<BFSPlayer>();
  } else if (player_type == player_type_e::RANDOM) {
    return std::make_unique<RandomSPlayer>();
  } else {
    return nullptr;
  }
}

std::vector<direction_e> get_valid_directions(const Level* level, const TilePos& loc) {
  std::vector<direction_e> result;

  for (const direction_e& dir : { NORTH, EAST, SOUTH, WEST }) {
    TilePos neighbor;
    auto [dir_row, dir_col] = dir_map[dir];

    neighbor.row = loc.row + dir_row;
    neighbor.col = loc.col + dir_col;

    if (level->in_board(neighbor) && (level->is_free(neighbor) || level->is_food(neighbor))) {
      result.emplace_back(dir);
    }
  }

  return result;
}

direction_e SPlayer::get_random_dir() {
  TilePos snake_head{ snake->head() };
  std::cout << snake_head.row << " " << snake_head.col << '\n';
  auto possible_moves{ get_valid_directions(current_level, snake_head) };

  if (!possible_moves.empty()) {
    if (possible_moves.size() == 1) {
      return possible_moves[0];
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> range(0, possible_moves.size() - 1);
    int random_tile_idx = range(gen);

    return possible_moves[random_tile_idx];
  }

  return INVALID;
}

direction_e RandomSPlayer::next_move() {
  return get_random_dir();
}

void RandomSPlayer::bind_snake(Snake* s) { snake = s; }

player_type_e RandomSPlayer::type() const { return player_type_e::RANDOM; }

void RandomSPlayer::bind_level(Level* l) { current_level = l; };

void BFSPlayer::bind_level(Level* l) { current_level = l; };

void BFSPlayer::bind_snake(Snake* s) { snake = s; };

player_type_e BFSPlayer::type() const { return player_type_e::BACKTRACKING; };

std::string dir_to_str(direction_e dir) {
  if (dir == direction_e::EAST) {
    return "E";
  } else if (dir == direction_e::WEST) {
    return "W";
  } else if (dir == direction_e::NORTH) {
    return "N";
  } else if (dir == direction_e::SOUTH) {
    return "S";
  } else if (dir == direction_e::NONE) {
    return "SP";
}}

std::string PathUnit::get_key() {
  std::string key;
  key.append(dir_to_str(snake.current_dir));

  key.append(",(");
    key.append(std::to_string(snake.head().row));
    key.append(",");
    key.append(std::to_string(snake.head().col));
    key.append(")");

  for (auto body_part : snake.body()) {
    key.append(",(");
    key.append(std::to_string(body_part.row));
    key.append(",");
    key.append(std::to_string(body_part.col));
    key.append(")");
  }

  return key;
}

TilePos moveto(TilePos snake_head, direction_e direction) {
  switch (direction) {
  case direction_e::NORTH:
    return TilePos{ snake_head.row - 1, snake_head.col };
    break;
  case direction_e::SOUTH:
    return TilePos{ snake_head.row + 1, snake_head.col };
    break;
  case direction_e::EAST:
    return TilePos{ snake_head.row, snake_head.col + 1 };
    break;
  case direction_e::WEST:
    return TilePos{ snake_head.row, snake_head.col - 1 };
    break;
  default:
    break;
  }
}

std::deque<direction_e> BFSPlayer::path_finder() {
  std::queue<PathUnit> places_to_visit;
  std::set<std::string> inspected;
  auto snakebfs = SnazeSimulation::get_snake();
  current_level->remove_snake(&snakebfs);
  // 2-- Guarda a localização atual para visitar.
  PathUnit spawn_loc{ snakebfs, {} };
  places_to_visit.push(spawn_loc);
  inspected.insert(spawn_loc.get_key());  // Chave de sua escolha.
  // 3-- Laço p/ identificar os locais alcançáveis mas ainda não explorados.
  while (not places_to_visit.empty()) {
    // 3.1-- Recupere um local para explorar agora.
    auto current_loc = places_to_visit.front();
    places_to_visit.pop();
    Snake& csnake = current_loc.snake;  // Apenas um alias.
    // 3.2-- Posicione a snake no level, p/ identificar p/ onde ela poderia ir.
    current_level->place_snake(&csnake);
    // 3.3-- Analise cada posição alcançável a partir da atual.
    for (direction_e dir : { NORTH, EAST, SOUTH, WEST }) {
      auto destination = moveto(csnake.head(), dir);  // Coord. a explorar.
      if (current_level->is_food(destination)) {      // É comida?
        current_loc.path.push_back(dir);              // Adicionar direção à solução.
        current_level->remove_snake(&csnake);
        return current_loc.path;                    // Retornar a solução.
      }
      // Se livre, precisamos guardar..
      if (current_level->is_free(destination)) {
        PathUnit new_loc{ current_loc };  // Clone p/ simular movimento da cobra
        new_loc.snake.move_to(dir);
        new_loc.path.push_back(dir);  // Adicione a direção ao caminho.
        // Este local já foi registrado antes?
        if (inspected.count(new_loc.get_key()) == 0) {
          places_to_visit.push(new_loc);        // Visitar no futuro.
          inspected.insert(new_loc.get_key());  // Marcar p/ não revisitar.
        }
      }
    }
    // 3.4-- Remove snake temporária p/ próxima exploração.
    current_level->remove_snake(&csnake);
  }
  return {};  // Sem solução!!!
}

direction_e BFSPlayer::next_move() {

  if (solution.empty()) {
    if (!random_mode) {
      solution = path_finder();
    }

    // If new solution is empty
    if (solution.empty()) {
      random_mode = true;
    }
  }

  if (random_mode) {
    return get_random_dir();
  } else {
    direction_e next_dir = solution.front();
    solution.pop_front();
    return next_dir;
  }
}
