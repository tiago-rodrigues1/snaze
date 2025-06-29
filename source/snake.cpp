#include "snake.hpp"


void Snake::move(){

    int prev_x_tail = ST_x[0];
    int prev_y_tail = ST_y[0];
    
    ST_x[0] = SH_x;
    ST_y[0] = SH_y;

    for(int i{1}; i < ST_len; ++i){
        int prev_prev_x_tail = ST_x[i];
        int prev_prev_y_tail = ST_y[i];

        ST_x[i] = prev_x_tail;
        ST_y[i] = prev_y_tail;

        prev_x_tail = prev_prev_x_tail;
        prev_y_tail = prev_prev_y_tail;
    }

    switch (actual_direction)
    {
    case snake_directions_e::UP:
        SH_y++;
        break;
    case snake_directions_e::DOWN:
        SH_y--;
        break;
    case snake_directions_e::RIGHT:
        SH_x++;
        break;
    case snake_directions_e::LEFT:
        SH_x--;
        break;
    default:
        break;
    }
}

void Snake::grow(){
    ST_len++;
}