#include "engine.h"

#include <curses.h>

const char *horse = 
"^__^\n"
"(..)\n"
"|_/ \\_______\n"
"   \\        )\\\n"
"    ||-----|| \\\n"
"    ||     ||\n"
"    ^^     ^^";

u8 gameInit(Context *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    ctx->state = GAME;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    return 0;
}

u8 gameUpdate(Context *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    clear();
    mvprintw(0, 0, horse);
    mvprintw(10, 0, "Press 'q' to quit.");
    refresh();

    int ch = getch();
    if (ch == 'q' || ch == 'Q') {
        ctx->state = MAIN_MENU;
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
