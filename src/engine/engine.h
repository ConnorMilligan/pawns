#ifndef ENGINE_H
#define ENGINE_H

#include "util.h"

enum GameState {
    MAIN_MENU,
    GAME,
};

typedef struct {
    enum GameState state;
} Context;

// --- Engine functions ---
// Context
u8 contextBuild(Context *ctx);
u8 contextDestroy(Context *ctx);

// Game
u8 gameInit(Context *ctx);
u8 gameUpdate(Context *ctx);
u8 gameDestroy(Context *ctx);

#endif // ENGINE_H
