#include "engine.h"

uint8_t contextBuild(Context *ctx, WINDOW *stdscr) {
    if (ctx == NULL) {
        return 1;
    }

    // Set fields
    ctx->windows[WIN_STDSCR] = stdscr;
    ctx->windows[WIN_CANVAS] = newwin(LINES -2, COLS - 2, 1, 1);

    for (size_t i = 0; i < MAX_PAWNS; i++) {
        ctx->pawns[i] = pawnBuild(32);
    }

    // Set state
    ctx->state = GS_GAME;
    ctx->isPaused = false;

    return 0;
}

uint8_t contextAddPawn(Context *ctx) {
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

uint8_t contextDrawPawns(Context *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    for (size_t i = 0; i < MAX_PAWNS; i++) {
        if (ctx->pawns[i].symbol != 32) {
            if (pawnDraw(&ctx->pawns[i], ctx->windows[WIN_CANVAS]) != 0) {
                return 1;
            }
        }
    }

    return 0;
}

uint8_t contextRefresh(Context *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    for (size_t i = 0; i < WIN_COUNT; i++) {
        wrefresh(ctx->windows[i]);
    }

    return 0;
}

uint8_t contextPause(Context *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    if (ctx->state == GS_GAME) {
        ctx->isPaused = !ctx->isPaused;
    }

    return 0;
}

uint8_t contextDestroy(Context *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    for (size_t i = 1; i < WIN_COUNT; i++) {
        delwin(ctx->windows[i]);
    }

    return 0;
}
