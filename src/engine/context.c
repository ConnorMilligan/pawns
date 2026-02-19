#include "engine.h"

uint8_t contextBuild(Context *ctx, WINDOW *stdscr) {
    if (ctx == NULL) {
        return 1;
    }

    getmaxyx(stdscr, ctx->rows, ctx->cols);

    // Set fields
    ctx->windows[WIN_STDSCR] = stdscr;
    ctx->windows[WIN_CANVAS] = newwin(ctx->rows-2, ctx->cols-2, 1, 1);

    // Add population
    ctx->pop = populationBuild();

    // Build tiles according to the size of the canvas
    ctx->map = mapBuild(ctx->rows-2, ctx->cols-2);

    // Set state
    ctx->state = GS_GAME;
    ctx->isPaused = false;

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

    if (mapDestroy(&ctx->map) != 0) {
        return 1;
    }

    return 0;
}
