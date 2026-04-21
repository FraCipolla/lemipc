#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>

typedef struct { int x; int y; } t_pos;

typedef struct s_player {
    size_t              current;
    size_t              team;
    t_game_state        *game_state;
    t_pos               pos;
    struct s_player     *next;
}   t_player;

typedef struct s_team {
    size_t          team_idx;
    size_t          team_size;
    t_player        *players;
    struct s_team   *next;
}   t_team;

typedef struct s_board {
    size_t      width;
    size_t      height;
    t_player    *players;
}   t_board;

typedef struct s_game_state {
    short       start;
    t_board     *board;
    size_t      n_players;
    size_t      n_teams;
    t_team      *teams;
}   t_game_state;

#endif