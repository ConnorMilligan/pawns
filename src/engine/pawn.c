#include "engine.h"

Pawn pawnBuild(const char symbol) {
    return (Pawn) {
        .symbol = symbol,

        .pos = {0, 0},
    };
}

u8 pawnMove(Pawn *pawn, i8 dx, i8 dy) {
    if (pawn == NULL) {
        return 1;
    }

    if (pawn->pos.x + dx > 0 && pawn->pos.x + dx < COLS - 2) {
        pawn->pos.x += dx;
    }
    if (pawn->pos.y + dy > 0 && pawn->pos.y + dy < LINES - 2) {
        pawn->pos.y += dy;
    }

    return 0;
}

u8 pawnDraw(Pawn *pawn, WINDOW *win) {
    if (pawn == NULL || win == NULL) {
        return 1;
    }

    mvwaddch(win, pawn->pos.y, pawn->pos.x, pawn->symbol);

    return 0;
}