#include <curses.h>

const char *horse = 
"^__^\n"
"(..)\n"
"|_/ \\_______\n"
"   \\        )\\\n"
"    ||-----|| \\\n"
"    ||     ||\n"
"    ^^     ^^";

int main(int argc, char **argv) {    
    initscr();
    curs_set(0);
    noecho();

    printw("%s", horse);
    refresh();
    getch();
    endwin();

    return 0;
}
