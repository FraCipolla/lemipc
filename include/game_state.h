#ifndef GAME_STATE_H
#define GAME_STATE_H

typedef struct s_game_state {
    size_t  **board;
    size_t  max_player;
    size_t  player_turn;
}   game_state_t;

#endif