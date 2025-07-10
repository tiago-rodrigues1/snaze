#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <memory>

#include "snake.hpp"
#include "level.hpp"

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
  virtual bool path_finder() { return false; };  //??
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


#endif