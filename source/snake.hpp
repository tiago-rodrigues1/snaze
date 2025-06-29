#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <cstdint>

enum snake_directions_e : std::uint8_t{
  STOP = 0,
  RIGHT,
  LEFT, 
  UP, 
  DOWN
};                    


class Snake{
  private:
  int SH_x;
  int SH_y;
  int ST_x[100];
  int ST_y[100];
  int ST_len;
  snake_directions_e actual_direction;
  
  public:
  int lives;
  void move();
  void grow();

};


#endif