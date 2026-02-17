#include "engine.h"

#include <curses.h>

u8 gameDraw(Context *ctx);
u8 gameHandleInput(Context *ctx);

u8 gameInit(Context *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (contextBuild(ctx, stdscr) != 0) {
        endwin();
        return 1;
    }

    ctx->state = GAME;

    return 0;
}

u8 gameUpdate(Context *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    if (gameDraw(ctx) != 0) {
        return 1;
    }
    if (gameHandleInput(ctx) != 0) {
        return 1;
    }

    return 0;
}

u8 gameDraw(Context *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    clear();

    switch (ctx->state) {
        case MAIN_MENU:
            break;
        case GAME:
            menuDrawBorder(ctx);
            break;
    }

    refresh();

    return 0;
}

u8 gameHandleInput(Context *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    int ch = getch();

    switch (ch) {
        case 'q':
            ctx->state = QUIT;
            break;
        default:
            break;
    }

    return 0;
}


u8 gameDestroy(Context *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    endwin();

    return 0;
}
