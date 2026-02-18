#include "engine.h"

u8 contextBuild(Context *ctx, WINDOW *stdscr) {
    if (ctx == NULL) {
        return 1;
    }

    // Set fields
    ctx->stdscr = stdscr;
    ctx->canvas = newwin(LINES - 2, COLS - 2, 1, 1);

    for (size_t i = 0; i < MAX_PAWNS; i++) {
        ctx->pawns[i] = pawnBuild(32);
    }

    // Set state
    ctx->state = GAME;
    ctx->isPaused = false;

    return 0;
}

u8 contextAddPawn(Context *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    for (size_t i = 0; i < MAX_PAWNS; i++) {
        if (ctx->pawns[i].symbol == 32) {
            ctx->pawns[i] = pawnBuild('@');
            break;
        }
    }

    return 0;
}

u8 contextDrawPawns(Context *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    for (size_t i = 0; i < MAX_PAWNS; i++) {
        if (ctx->pawns[i].symbol != 32) {
            if (pawnDraw(&ctx->pawns[i], ctx->canvas) != 0) {
                return 1;
            }
        }
    }

    return 0;
}

u8 contextPause(Context *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    if (ctx->state == GAME) {
        ctx->isPaused = !ctx->isPaused;
    }

    return 0;
}

u8 contextDestroy(Context *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    // Currently nothing to clean up

    return 0;
}
