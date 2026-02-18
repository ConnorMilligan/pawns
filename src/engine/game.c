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

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    }

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
    if (!ctx->isPaused && gameTick(ctx) != 0) {
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
            if (ctx->isPaused) {
                attron(COLOR_PAIR(1));
                mvwaddstr(ctx->canvas, LINES / 2 - 1, COLS / 2 - 5, "* PAUSED *");///
                attroff(COLOR_PAIR(1));
            }
            break;
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
        case ' ':
            contextPause(ctx);
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
