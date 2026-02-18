#ifndef ENGINE_H
#define ENGINE_H

#include "util.h"

#include <curses.h>

#define MAX_PAWNS 10
#define TPS 10

enum GameState {
    GAME,
    QUIT,
};

typedef struct {
    u8 x;
    u8 y;
} Position;

typedef struct {
    char symbol;

    Position pos;
} Pawn;

typedef struct {
    enum GameState state;
    bool isPaused;

    Pawn pawns[MAX_PAWNS];

    WINDOW *stdscr;
    WINDOW *canvas;
} Context;

// --- Engine functions ---
// Context
u8 contextBuild(Context *ctx, WINDOW *stdscr);
u8 contextAddPawn(Context *ctx);
u8 contextDrawPawns(Context *ctx);
u8 contextPause(Context *ctx);
u8 contextDestroy(Context *ctx);

// Game
u8 gameInit(Context *ctx);
u8 gameUpdate(Context *ctx);
u8 gameDestroy(Context *ctx);

// Menu
u8 menuDrawBorder(WINDOW *win, const char *title);

// Pawn
Pawn pawnBuild(const char symbol);
u8 pawnMove(Pawn *pawn, i8 dx, i8 dy);
u8 pawnDraw(Pawn *pawn, WINDOW *win);


#endif // ENGINE_H
