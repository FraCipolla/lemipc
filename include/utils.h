#ifndef UTILS_H
#define UTILS_H

void cleanup();
bool is_game_over(t_game_state *gs, int8_t team);
bool surrend(t_game_state *gs, int8_t team, int8_t x, int8_t y);

#endif
