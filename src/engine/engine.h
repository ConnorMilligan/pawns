#ifndef ENGINE_H
#define ENGINE_H

#include "util.h"

#include <curses.h>

enum GameState {
    GAME,
    QUIT,
};

typedef struct {
    enum GameState state;

    WINDOW *stdscr;
    WINDOW *canvas;
} Context;

// --- Engine functions ---
// Context
u8 contextBuild(Context *ctx, WINDOW *stdscr);
u8 contextDestroy(Context *ctx);

// Game
u8 gameInit(Context *ctx);
u8 gameUpdate(Context *ctx);
u8 gameDestroy(Context *ctx);

// Menu
u8 menuDrawBorder(WINDOW *win, const char *title);

#endif // ENGINE_H
