#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <memory>

#include "snake.hpp"
#include "level.hpp"

class SPlayer {
protected:
  int m_score{0};
public:
  SPlayer() = default;
  virtual ~SPlayer() = default;

  static std::unique_ptr<SPlayer> create_player(player_type_e player_type);

  virtual direction_e next_move() = 0;
  virtual player_type_e type() const = 0;
  virtual bool path_finder() { return false; };  //??
  virtual void bind_level(Level* l) = 0;
  virtual void bind_snake(Snake* s) = 0;
  virtual void update_score() { ++m_score; };

  int score() const { return m_score; };
};

class RandomSPlayer : public SPlayer {
private:
  Level* current_level;
  Snake* snake;
public: 
  direction_e next_move() override;
  virtual void bind_level(Level* l) override;
  virtual void bind_snake(Snake* s) override;
  virtual player_type_e type() const override;
};


#endif