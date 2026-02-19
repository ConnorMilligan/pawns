#ifndef ENGINE_H
#define ENGINE_H

#include <curses.h>
#include <stdint.h>

#define TERM_COLS 80
#define TERM_ROWS 25

#define WIN_COUNT 2

#define MAX_PAWNS 30
#define TPS 15

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

enum TileID {
    TI_GRASS,
    TI_WALL
};

enum Windows {
    WIN_STDSCR,
    WIN_CANVAS,
};

typedef struct {
    uint8_t x;
    uint8_t y;
} Position;

#define MAX_PATH_LENGTH 256

typedef struct {
    char symbol;
    Position pos;
    Position pathTarget;
    bool isPathfinding;
    Position pathCache[MAX_PATH_LENGTH];
    uint16_t pathLength;
    uint16_t pathIndex; // current position in path
} Pawn;

typedef struct {
    Pawn pawns[MAX_PAWNS];
    uint16_t count;
} Population;

typedef struct {
    char symbol;
    const char *name;
    uint8_t color;

    Position pos;

    bool isSolid;
} Tile;

typedef struct {
    Tile **tiles;
    uint16_t rows, cols;
} Map;

typedef struct {
    enum GameState state;
    bool isPaused;

    Population pop;

    uint16_t rows, cols;

    WINDOW *windows[WIN_COUNT];
    Map map;
} Context;

// --- Engine functions ---
// Context
uint8_t contextBuild(Context *ctx, WINDOW *stdscr);
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
uint8_t pawnMovePath(Pawn *pawn, Map *map, Population *pop, Position target);
uint8_t pawnComputePathFull(Pawn *pawn, Map *map, Population *pop, Position goal);

// Population
Population populationBuild();
uint8_t populationAddPawn(Population *pop);
uint8_t populationDraw(Population *pop, WINDOW *win);

// Tile
Tile tileBuild(enum TileID id);
uint8_t tileDraw(Tile *tile, WINDOW *win);

// Map
Map mapBuild(uint16_t rows, uint16_t cols);
Tile* mapGetTile(Map *map, Position pos);
uint8_t mapDraw(Map *map, WINDOW *win);
uint8_t mapDestroy(Map *map);


#endif // ENGINE_H
