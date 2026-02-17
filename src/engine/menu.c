#include "engine.h"

u8 menuDrawBorder(Context *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    int maxY, maxX;
    getmaxyx(ctx->stdscr, maxY, maxX);

    // Draw border
    for (int x = 0; x < maxX; x++) {
        mvaddch(0, x, ACS_HLINE);
        mvaddch(maxY - 1, x, ACS_HLINE);
    }
    for (int y = 0; y < maxY; y++) {
        mvaddch(y, 0, ACS_VLINE);
        mvaddch(y, maxX - 1, ACS_VLINE);
    }
    mvaddch(0, 0, ACS_ULCORNER);
    mvaddch(0, maxX - 1, ACS_URCORNER);
    mvaddch(maxY - 1, 0, ACS_LLCORNER);
    mvaddch(maxY - 1, maxX - 1, ACS_LRCORNER);

    mvaddch(0, 1, ACS_RTEE);
    mvaddch(0, 7, ACS_LTEE);
    mvaddnstr(0, 2, "PAWNS", 5);

    return 0;
}