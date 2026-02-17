#include <curses.h>

#include "engine/engine.h"

int main(int argc, char **argv) {    
    Context ctx = {};

    if (gameInit(&ctx) != 0) {
        contextDestroy(&ctx);
        return 1;
    }

    while (ctx.state != QUIT) {
        gameUpdate(&ctx);
    }

    gameDestroy(&ctx);

    return 0;
}
