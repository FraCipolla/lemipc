#include <utils.h>

void cleanup() {
}

bool is_game_over(t_game_state *gs, int8_t team) {
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            if (gs->board[i][j] != -1 && gs->board[i][j] != team)
                return false;
        }
    }
    return true;
}

bool surrend(t_game_state *gs, int8_t team, int8_t x, int8_t y) {
    int8_t surrounding_enemies = 0;

    if (y < 256 && gs->board[y + 1][x] != -1 && gs->board[y + 1][x] != team)
        surrounding_enemies += 1;
    if (y < 256 && x < 256 && gs->board[y + 1][x + 1] != -1 && gs->board[y + 1][x + 1] != team)
        surrounding_enemies += 1;
    if (x < 256 && gs->board[y][x + 1] != -1 && gs->board[y][x + 1] != team)
        surrounding_enemies += 1;
    if (y > 0 && x < 256 && gs->board[y - 1][x + 1] != -1 && gs->board[y - 1][x + 1] != team)
        surrounding_enemies += 1;
    if (y > 0 && gs->board[y - 1][x] != -1 && gs->board[y - 1][x] != team)
        surrounding_enemies += 1;
    if (y > 0 && x > 0 && gs->board[y - 1][x - 1] != -1 && gs->board[y - 1][x - 1] != team)
        surrounding_enemies += 1;
    if (x > 0 && gs->board[y][x - 1] != -1 && gs->board[y][x - 1] != team)
        surrounding_enemies += 1;
    if (y < 256 && x > 0 && gs->board[y + 1][x - 1] != -1 && gs->board[y + 1][x - 1] != team)
        surrounding_enemies += 1;
    return surrounding_enemies > 1;
}
