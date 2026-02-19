#include "engine/engine.h"

int main(int argc, char **argv) {    
    Context ctx = {};

    if (gameInit(&ctx) != 0) {
        contextDestroy(&ctx);
        return 1;
    }

    while (ctx.state != GS_QUIT) {
        gameUpdate(&ctx);
        napms(1000 / TPS);
    }

    gameDestroy(&ctx);

    return 0;
}
