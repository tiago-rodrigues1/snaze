#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "snake.hpp"
#include "level.hpp"

enum player_type_e : std::uint8_t { RANDOM = 0, BACKTRACKING, HUMAN };

class SPlayer {
public:
  SPlayer() = default;
  virtual ~SPlayer() = default;
  virtual MoveDir next_move() = 0;
  virtual player_type_e type() const = 0;
  virtual bool path_finder() { return false; };  //??
};

class RandomSPlayer : public SPlayer {
  public: 
  Level* running_level;
  Snake* snake;
  MoveDir next_move(std::vector<std::string>& board);
  void bind_level();
  void bind_snake(Snake* s);
};


#endif