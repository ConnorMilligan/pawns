#include "engine.h"

Population populationBuild() {
    Population pop = {
        .count = 0,
    };

    for (size_t i = 0; i < MAX_PAWNS; i++) {
        pop.pawns[i] = pawnBuild(32);
    }

    return pop;
}

uint8_t populationAddPawn(Population *pop) {
    if (pop == NULL) {
        return 1;
    }

    if (pop->count >= MAX_PAWNS) {
        return 1;
    }

    pop->pawns[pop->count] = pawnBuild('@');
    pop->count++;

    return 0;
}

uint8_t populationDraw(Population *pop, WINDOW *win) {
    if (pop == NULL || win == NULL) {
        return 1;
    }

    for (size_t i = 0; i < pop->count; i++) {
        if (pawnDraw(&pop->pawns[i], win) != 0) {
            return 1;
        }
    }

    return 0;
}