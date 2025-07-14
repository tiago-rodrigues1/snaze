#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <memory>
#include <stack> // Add this include for std::stack
#include <deque>

#include "snake.hpp"
#include "level.hpp"
#include "common.hpp"

class SPlayer {
protected:
  int m_score{0};
  Level* current_level;
  Snake* snake;
public:
  SPlayer() = default;
  virtual ~SPlayer() = default;

  static std::unique_ptr<SPlayer> create_player(player_type_e player_type);

  virtual MoveDir next_move(std::vector<std::string>& board) = 0;
  virtual player_type_e type() const = 0;
  virtual void bind_level(Level* l) = 0;
  virtual void bind_snake(Snake* s) = 0;

  int score() const { return m_score; };
};

class RandomSPlayer : public SPlayer {
public: 
  MoveDir next_move(std::vector<std::string>& board) override;
  virtual void bind_level(Level* l) override;
  virtual void bind_snake(Snake* s) override;
  virtual player_type_e type() const override;
};
// Forward declare PathUnit if not already defined
struct PathUnit
{
  Snake snake;
  std::deque<Dir> path;
  std::string get_key();
};

class BFSPlayer : public SPlayer {
public: 
  std::deque<MoveDir> solution;
  std::deque<MoveDir>::iterator current_move;
  MoveDir next_move(std::vector<std::string>& board) override;
  virtual void bind_level(Level* l) override;
  virtual void bind_snake(Snake* s) override;
  virtual player_type_e type() const override;
  std::deque<Dir> path_finder();
};


#endif