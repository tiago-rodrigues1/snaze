#include "player.hpp"
#include <cstdint>
#include <queue>
#include <random>
#include <set>

std::unique_ptr<SPlayer> SPlayer::create_player(player_type_e player_type) {
  if (player_type == player_type_e::BACKTRACKING) {
    return nullptr;
  } else if (player_type == player_type_e::RANDOM) {
    return std::make_unique<RandomSPlayer>();
  } else {
    return nullptr;
  }
}

MoveDir RandomSPlayer::next_move(std::vector<std::string>& board) {
  MoveDir actual_dir = snake->actual_direction;
  TilePos snake_head = snake->body.back();
  std::vector<MoveDir> possible_moves;
  MoveDir next_move;

  size_t x = snake_head.col;
  size_t y = snake_head.row;

  if (x + 1 < board[0].size() && board[y][x + 1] != '#' && board[y][x + 1] != '.') {
    possible_moves.push_back({ 1, 0 });
  }

  if (x > 0 && board[y][x - 1] != '#' && board[y][x - 1] != '.') {
    possible_moves.push_back({ -1, 0 });
  }

  if (y + 1 < board.size() && board[y + 1][x] != '#' && board[y + 1][x] != '.') {
    possible_moves.push_back({ 0, -1 });
  }

  if (y > 0 && board[y - 1][x] != '#' && board[y - 1][x] != '.') {
    possible_moves.push_back({ 0, 1 });
  }

  if (!possible_moves.empty()) {
    if (possible_moves.size() == 1) {
      next_move = possible_moves[0];
    } else {
      std::random_device rd;
      std::mt19937 gen(rd());

      std::uniform_int_distribution<> range(0, possible_moves.size() - 1);

      int random_tile_idx = range(gen);

      next_move = possible_moves[random_tile_idx];
    }
  } else {
    next_move = actual_dir;
  }

  return next_move;
}

void RandomSPlayer::bind_snake(Snake* s) { snake = s; }

player_type_e RandomSPlayer::type() const { return player_type_e::RANDOM; }

void RandomSPlayer::bind_level(Level* l) { current_level = l; };

void BFSPlayer::bind_level(Level* l) { current_level = l; };

void BFSPlayer::bind_snake(Snake* s) { snake = s; };

player_type_e BFSPlayer::type() const { return player_type_e::BACKTRACKING; };


std::string move_dir_to_str(MoveDir move_dir) {
  if (move_dir.dx == 1 && move_dir.dy == 0) {
    return "L";
  } else if (move_dir.dx == -1 && move_dir.dy == 0) {
    return "O";
  } else if (move_dir.dx == 0 && move_dir.dy == 1) {
    return "N";
  } else if (move_dir.dx == 0 && move_dir.dy == -1) {
    return "S";
  }
}

std::string PathUnit::get_key() {
  std::string key;
  key.append(move_dir_to_str(snake.actual_direction));

  for (auto body_part : snake.body) {
    key.append(",(");
    key.append(std::to_string(body_part.row));
    key.append(",");
    key.append(std::to_string(body_part.col));
    key.append(")");
  }

  return key;
}

TilePos moveto(TilePos snake_head, Dir direction){
  switch (direction)
  {
  case Dir::N:
    return TilePos{snake_head.row - 1, snake_head.col};
  break;
  case Dir::S:
    return TilePos{snake_head.row + 1, snake_head.col};
  break;
  case Dir::L:
    return TilePos{snake_head.row, snake_head.col + 1};
  break;
  case Dir::O:
    return TilePos{snake_head.row, snake_head.col - 1};
  break;
  default:
    break;
  }
}


std::deque<Dir> BFSPlayer::path_finder() {
  std::queue<PathUnit> places_to_visit;
  std::set<std::string> inspected;
  current_level->remove_snake(*snake);
  // 2-- Guarda a localização atual para visitar.
  PathUnit spawn_loc{ *snake, {} };
  places_to_visit.push(spawn_loc);
  inspected.insert(spawn_loc.get_key());  // Chave de sua escolha.
  // 3-- Laço p/ identificar os locais alcançáveis mas ainda não explorados.
  while (not places_to_visit.empty()) {
    // 3.1-- Recupere um local para explorar agora.
    auto current_loc = places_to_visit.front();
    places_to_visit.pop();
    Snake& csnake = current_loc.snake;  // Apenas um alias.
    // 3.2-- Posicione a snake no level, p/ identificar p/ onde ela poderia ir.
    current_level->set_snake(csnake);
    // 3.3-- Analise cada posição alcançável a partir da atual.
    for (Dir dir : { N, S, L, O }) {
      auto destination = moveto(csnake.body.back(), dir);  // Coord. a explorar.
      if (current_level->is_food(destination)) {           // É comida?
        current_loc.path.push_back(dir);                   // Adicionar direção à solução.
        return current_loc.path;                            // Retornar a solução.
      }
      // Se livre, precisamos guardar..
      if (current_level->is_free(destination)) {
        PathUnit new_loc{ current_loc };      // Clone p/ simular movimento da cobra
        new_loc.snake.move_to(dir);  // Mova a (cópia da) cobra.
        new_loc.path.push_back(dir);              // Adicione a direção ao caminho.
        // Este local já foi registrado antes?
        if (inspected.count(new_loc.get_key()) == 0) {
          places_to_visit.push(new_loc);        // Visitar no futuro.
          inspected.insert(new_loc.get_key());  // Marcar p/ não revisitar.
        }
      }
    }
    // 3.4-- Remove snake temporária p/ próxima exploração.
    current_level->remove_snake(csnake);
  }
  return {};  // Sem solução!!!
}
