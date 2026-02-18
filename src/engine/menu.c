#include "engine.h"

#include <string.h>

u8 menuDrawBorder(WINDOW *win, const char *title) {
    if (win == NULL || title == NULL) {
        return 1;
    }
    
    wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
    mvwaddnstr(win, 0, 2, title, -1);
    mvwaddch(win, 0, 1, ACS_RTEE);
    mvwaddch(win, 0, 2 + strlen(title), ACS_LTEE);

    return 0;
}