#include "engine.h"

u8 contextBuild(Context *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    ctx->state = MAIN_MENU;

    return 0;
}

u8 contextDestroy(Context *ctx) {
    if (ctx == NULL) {
        return 1;
    }

    // Currently nothing to clean up

    return 0;
}
