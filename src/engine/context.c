#include "engine.h"

u8 contextBuild(Context *ctx, WINDOW *stdscr) {
    if (ctx == NULL) {
        return 1;
    }

    // Set fields
    ctx->stdscr = stdscr;
    ctx->canvas = newwin(LINES - 2, COLS - 2, 1, 1);

    // Set state
    ctx->state = GAME;

    return 0;
}

u8 contextDestroy(Context *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    // Currently nothing to clean up

    return 0;
}
