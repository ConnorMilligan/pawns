#include "engine.h"
#include <stdlib.h>
#include <math.h>

static uint8_t isValidPos(Position pos) {
    return pos.x > 0 && pos.x < TERM_COLS - 2 && pos.y > 0 && pos.y < TERM_ROWS - 2;
}

static Position getNextStep(Position start, Position goal) {
    Position next = start;
    
    if (start.x < goal.x) next.x++;
    else if (start.x > goal.x) next.x--;
    
    if (start.y < goal.y) next.y++;
    else if (start.y > goal.y) next.y--;
    
    if (!isValidPos(next)) {
        next = start;
        if (start.x < goal.x) next.x++;
        else if (start.x > goal.x) next.x--;
    }
    
    if (!isValidPos(next)) {
        next = start;
        if (start.y < goal.y) next.y++;
        else if (start.y > goal.y) next.y--;
    }
    
    return next;
}

Pawn pawnBuild(const char symbol) {
    return (Pawn) {
        .symbol = symbol,
        .pos = {TERM_COLS / 2, TERM_ROWS / 2},
        .pathTarget = {TERM_COLS / 2, TERM_ROWS / 2},
        .isPathfinding = false,
    };
}

uint8_t pawnMove(Pawn *pawn, int8_t dx, int8_t dy) {
    if (pawn == NULL) {
        return 1;
    }

    if (pawn->pos.x + dx > 0 && pawn->pos.x + dx < TERM_COLS - 2) {
        pawn->pos.x += dx;
    }
    if (pawn->pos.y + dy > 0 && pawn->pos.y + dy < TERM_ROWS - 2) {
        pawn->pos.y += dy;
    }

    return 0;
}

uint8_t pawnDraw(Pawn *pawn, WINDOW *win) {
    if (pawn == NULL || win == NULL) {
        return 1;
    }

    mvwaddch(win, pawn->pos.y, pawn->pos.x, pawn->symbol);

    return 0;
}

uint8_t pawnMovePath(Pawn *pawn, Position target) {
    if (pawn == NULL) {
        return 1;
    }

    if (!isValidPos(target)) {
        return 1;
    }

    // If already at target, stop pathfinding
    if (pawn->pos.x == target.x && pawn->pos.y == target.y) {
        pawn->isPathfinding = false;
        return 0;
    }

    // Set new target if different
    if (target.x != pawn->pathTarget.x || target.y != pawn->pathTarget.y) {
        pawn->pathTarget = target;
        pawn->isPathfinding = true;
    }

    Position nextStep = getNextStep(pawn->pos, target);

    int8_t dx = nextStep.x - pawn->pos.x;
    int8_t dy = nextStep.y - pawn->pos.y;

    pawnMove(pawn, dx, dy);

    return 0;
}