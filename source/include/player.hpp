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
  Level* current_level;
  Snake* snake;
  int m_score{0};

  direction_e get_random_dir();
public:
  SPlayer() = default;
  virtual ~SPlayer() = default;

  static std::unique_ptr<SPlayer> create_player(player_type_e player_type);

  virtual direction_e next_move() = 0;
  virtual player_type_e type() const = 0;
  virtual void bind_level(Level* l) = 0;
  virtual void bind_snake(Snake* s) = 0;
  virtual void update_score() { m_score += 50; };

  int score() const { return m_score; };
};

class RandomSPlayer : public SPlayer {
public: 
  direction_e next_move() override;
  virtual void bind_level(Level* l) override;
  virtual void bind_snake(Snake* s) override;
  virtual player_type_e type() const override;
};
// Forward declare PathUnit if not already defined


class BFSPlayer : public SPlayer {
private:
  bool random_mode{false};
public: 
  std::deque<direction_e> solution;
  std::deque<direction_e>::iterator current_move;
  direction_e next_move() override;
  virtual void bind_level(Level* l) override;
  virtual void bind_snake(Snake* s) override;
  virtual player_type_e type() const override;
  std::deque<direction_e> path_finder();
};


#endif