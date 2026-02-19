#include "engine.h"

#include <curses.h>
#include <stdlib.h>

uint8_t gameDraw(Context *ctx);
uint8_t gameDrawChangeState(Context *ctx);
uint8_t gameHandleInput(Context *ctx);
uint8_t gameTick(Context *ctx);

uint8_t gameInit(Context *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);

    // Check terminal size
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    if (rows < TERM_ROWS || cols < TERM_COLS) {
        endwin();
        fprintf(stderr, "Terminal size must be at least %dx%d\n", TERM_COLS, TERM_ROWS);
        return 1;
    }

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
        init_pair(2, COLOR_RED, COLOR_BLACK);
        init_pair(3, COLOR_GREEN, COLOR_BLACK);
        init_pair(4, COLOR_YELLOW, COLOR_BLACK);
        init_pair(5, COLOR_BLUE, COLOR_BLACK);
        init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(7, COLOR_CYAN, COLOR_BLACK);
    }

    if (contextBuild(ctx, stdscr) != 0) {
        endwin();
        return 1;
    }
    
    gameDrawChangeState(ctx);
    return 0;
}

uint8_t gameUpdate(Context *ctx) {
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

uint8_t gameDraw(Context *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    werase(ctx->windows[WIN_CANVAS]);

    switch (ctx->state) {
        case GS_GAME:
            contextDrawPawns(ctx);
            if (ctx->isPaused) {
                // Draw paused message in yellow
                wattron(ctx->windows[WIN_CANVAS], COLOR_PAIR(CP_YELLOW) | A_BOLD | A_BLINK);
                mvwprintw(ctx->windows[WIN_CANVAS], LINES / 2 - 1, (COLS - 10) / 2, "* PAUSED *");
                wattroff(ctx->windows[WIN_CANVAS], COLOR_PAIR(CP_YELLOW) | A_BOLD | A_BLINK);
            }
            break;
        case GS_QUIT:
            break;
    }

    wrefresh(ctx->windows[WIN_CANVAS]);

    return 0;
}

uint8_t gameHandleInput(Context *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    int ch = getch();
    if (ch == ERR) {
        return 0;
    }

    switch (ch) {
        case 'q':
            ctx->state = GS_QUIT;
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

uint8_t gameTick(Context *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    for (size_t i = 0; i < MAX_PAWNS; i++) {
        if (ctx->pawns[i].symbol != 32) {
            if (!ctx->pawns[i].isPathfinding || 
                (ctx->pawns[i].pos.x == ctx->pawns[i].pathTarget.x && 
                 ctx->pawns[i].pos.y == ctx->pawns[i].pathTarget.y)) {
                Position target = {rand() % (TERM_COLS - 2) + 1, rand() % (TERM_ROWS - 2) + 1};
                pawnMovePath(&ctx->pawns[i], target);
            } else {
                pawnMovePath(&ctx->pawns[i], ctx->pawns[i].pathTarget);
            }
        }
    }

    return 0;
}

uint8_t gameDrawChangeState(Context *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    switch (ctx->state) {
        case GS_GAME:
            menuDrawBorder(ctx->windows[WIN_STDSCR], "PAWNS");
            break;
        case GS_QUIT:
            break;
    }

    contextRefresh(ctx);

    return 0;
}


uint8_t gameDestroy(Context *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    endwin();

    return 0;
}
