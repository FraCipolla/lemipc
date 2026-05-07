#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>
#include <semaphore.h>
#include <stdint.h>

typedef struct s_game_state {
    sem_t       play;
    int8_t      board[256][256];
}   t_game_state;

#endif
