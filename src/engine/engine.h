#ifndef ENGINE_H
#define ENGINE_H

#include <curses.h>

#define TERM_COLS 80
#define TERM_ROWS 25

#define WIN_COUNT 2

#define MAX_PAWNS 10
#define TPS 10

#define CP_WHITE 1
#define CP_RED 2
#define CP_GREEN 3
#define CP_YELLOW 4
#define CP_BLUE 5
#define CP_MAGENTA 6
#define CP_CYAN 7

enum GameState {
    GS_GAME,
    GS_QUIT,
};

enum Windows {
    WIN_STDSCR,
    WIN_CANVAS,
};

typedef struct {
    uint8_t x;
    uint8_t y;
} Position;

typedef struct {
    char symbol;
    Position pos;
    Position pathTarget;
    bool isPathfinding;
} Pawn;

typedef struct {
    enum GameState state;
    bool isPaused;

    Pawn pawns[MAX_PAWNS];

    WINDOW *windows[WIN_COUNT];
} Context;

// --- Engine functions ---
// Context
uint8_t contextBuild(Context *ctx, WINDOW *stdscr);
uint8_t contextAddPawn(Context *ctx);
uint8_t contextDrawPawns(Context *ctx);
uint8_t contextRefresh(Context *ctx);
uint8_t contextPause(Context *ctx);
uint8_t contextDestroy(Context *ctx);

// Game
uint8_t gameInit(Context *ctx);
uint8_t gameUpdate(Context *ctx);
uint8_t gameDestroy(Context *ctx);

// Menu
uint8_t menuDrawBorder(WINDOW *win, const char *title);

// Pawn
Pawn pawnBuild(const char symbol);
uint8_t pawnMove(Pawn *pawn, int8_t dx, int8_t dy);
uint8_t pawnDraw(Pawn *pawn, WINDOW *win);
uint8_t pawnMovePath(Pawn *pawn, Position target);


#endif // ENGINE_H
