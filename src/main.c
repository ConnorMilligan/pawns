#include <curses.h>

#include "engine/engine.h"

int main(int argc, char **argv) {    
    Context ctx;

    if (contextBuild(&ctx) != 0) {
        return 1;
    }

    if (gameInit(&ctx) != 0) {
        contextDestroy(&ctx);
        return 1;
    }

    while (ctx.state == GAME) {
        gameUpdate(&ctx);
    }

    gameDestroy(&ctx);

    return 0;
}
