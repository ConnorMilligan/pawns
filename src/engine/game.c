#include "engine.h"

#include <curses.h>
#include <stdlib.h>

u8 gameDraw(Context *ctx);
u8 gameDrawChangeState(Context *ctx);
u8 gameHandleInput(Context *ctx);
u8 gameTick(Context *ctx);

u8 gameInit(Context *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);

    if (contextBuild(ctx, stdscr) != 0) {
        endwin();
        return 1;
    }

    gameDrawChangeState(ctx);

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
    if (gameTick(ctx) != 0) {
        return 1;
    }

    return 0;
}

u8 gameDraw(Context *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    werase(ctx->canvas);

    switch (ctx->state) {
        case GAME:
            contextDrawPawns(ctx);
        case QUIT:
            break;
    }

    wrefresh(ctx->canvas);

    return 0;
}

u8 gameHandleInput(Context *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    int ch = getch();
    if (ch == ERR) {
        return 0;
    }

    switch (ch) {
        case 'q':
            ctx->state = QUIT;
            break;
        case 'a':
            contextAddPawn(ctx);
            break;
        default:
            break;
    }

    return 0;
}

u8 gameTick(Context *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    for (size_t i = 0; i < MAX_PAWNS; i++) {
        if (ctx->pawns[i].symbol != 32) {
            // Move pawn randomly
            i8 dx = (rand() % 3) - 1;
            i8 dy = (rand() % 3) - 1;
            pawnMove(&ctx->pawns[i], dx, dy);
        }
    }

    return 0;
}

u8 gameDrawChangeState(Context *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    switch (ctx->state) {
        case GAME:
            menuDrawBorder(ctx->stdscr, "PAWNS");
            break;
        case QUIT:
            break;
    }

    wrefresh(ctx->stdscr);

    return 0;
}


u8 gameDestroy(Context *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    endwin();

    return 0;
}
