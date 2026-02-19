#include "engine.h"

Pawn pawnBuild(const char symbol) {
    return (Pawn) {
        .symbol = symbol,

        .pos = {0, 0},
    };
}

uint8_t pawnMove(Pawn *pawn, int8_t dx, int8_t dy) {
    if (pawn == NULL) {
        return 1;
    }

    if (pawn->pos.x + dx > 0 && pawn->pos.x + dx < TERM_COLS - 2) {
        pawn->pos.x += dx;
    }
    if (pawn->pos.y + dy > 0 && pawn->pos.y + dy < TERM_ROWS - 2) {
        pawn->pos.y += dy;
    }

    return 0;
}

uint8_t pawnDraw(Pawn *pawn, WINDOW *win) {
    if (pawn == NULL || win == NULL) {
        return 1;
    }

    mvwaddch(win, pawn->pos.y, pawn->pos.x, pawn->symbol);

    return 0;
}