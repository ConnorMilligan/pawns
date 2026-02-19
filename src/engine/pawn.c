#include "engine.h"
#include <stdlib.h>
#include <math.h>
#include <float.h>

static inline int idx_of(Position p, uint16_t cols) {
    return p.y * cols + p.x;
}

// Validate position against map bounds and solidity
static uint8_t isValidPos(Position pos, Map *map) {
    if (map == NULL) return 0;
    if (pos.x < 0 || pos.x >= map->cols || pos.y < 0 || pos.y >= map->rows) return 0;
    Tile *t = mapGetTile(map, pos);
    if (t == NULL) return 0;
    return !t->isSolid;
}

// Simple A* to compute the next step from start towards goal avoiding solid tiles
static Position aStarNextStep(Map *map, Position start, Position goal) {
    Position empty = start;
    if (map == NULL) return empty;

    // If start or goal invalid return start
    if (!isValidPos(start, map) || !isValidPos(goal, map)) return start;

    uint32_t total = (uint32_t)map->rows * (uint32_t)map->cols;
    float *gScore = malloc(total * sizeof(float));
    float *fScore = malloc(total * sizeof(float));
    int *cameFrom = malloc(total * sizeof(int));
    unsigned char *openSet = malloc(total); // 0/1 flags
    unsigned char *closedSet = malloc(total);
    if (!gScore || !fScore || !cameFrom || !openSet || !closedSet) {
        free(gScore); free(fScore); free(cameFrom); free(openSet); free(closedSet);
        return start;
    }

    for (uint32_t i = 0; i < total; i++) {
        gScore[i] = INFINITY;
        fScore[i] = INFINITY;
        cameFrom[i] = -1;
        openSet[i] = 0;
        closedSet[i] = 0;
    }

    int startIdx = idx_of(start, map->cols);
    int goalIdx = idx_of(goal, map->cols);
    gScore[startIdx] = 0.0f;
    fScore[startIdx] = (float)(abs(start.x - goal.x) + abs(start.y - goal.y));
    openSet[startIdx] = 1;

    // helper to pick node in open set with lowest fScore
    while (1) {
        int current = -1;
        float bestF = INFINITY;
        for (uint32_t i = 0; i < total; i++) {
            if (openSet[i] && fScore[i] < bestF) {
                bestF = fScore[i];
                current = (int)i;
            }
        }
        if (current == -1) break; // open set empty

        if (current == goalIdx) {
            // reconstruct path, return the step after start
            int cur = current;
            int prev = cameFrom[cur];
            while (prev != -1 && prev != startIdx) {
                cur = prev;
                prev = cameFrom[cur];
            }
            Position out = start;
            if (cur != startIdx) {
                out.x = cur % map->cols;
                out.y = cur / map->cols;
            }
            free(gScore); free(fScore); free(cameFrom); free(openSet); free(closedSet);
            return out;
        }

        openSet[current] = 0;
        closedSet[current] = 1;

        int cx = current % map->cols;
        int cy = current / map->cols;

        Position neighbors[4] = {{cx+1, cy}, {cx-1, cy}, {cx, cy+1}, {cx, cy-1}};
        for (int ni = 0; ni < 4; ni++) {
            Position np = neighbors[ni];
            if (!isValidPos(np, map)) continue;
            int niidx = idx_of(np, map->cols);
            if (closedSet[niidx]) continue;
            float tentative = gScore[current] + 1.0f;
            if (!openSet[niidx]) openSet[niidx] = 1;
            else if (tentative >= gScore[niidx]) continue;
            cameFrom[niidx] = current;
            gScore[niidx] = tentative;
            fScore[niidx] = tentative + (float)(abs(np.x - goal.x) + abs(np.y - goal.y));
        }
    }

    free(gScore); free(fScore); free(cameFrom); free(openSet); free(closedSet);
    return start; // no path found
}

Pawn pawnBuild(const char symbol) {
    return (Pawn) {
        .symbol = symbol,
        .pos = {0, 0},
        .pathTarget = {0, 0},
        .isPathfinding = false,
    };
}

uint8_t pawnMove(Pawn *pawn, int8_t dx, int8_t dy) {
    if (pawn == NULL) return 1;
    pawn->pos.x = (uint8_t)(pawn->pos.x + dx);
    pawn->pos.y = (uint8_t)(pawn->pos.y + dy);
    return 0;
}

uint8_t pawnDraw(Pawn *pawn, WINDOW *win) {
    if (pawn == NULL || win == NULL) return 1;
    wattron(win, COLOR_PAIR(CP_YELLOW) | A_BOLD);
    mvwaddch(win, pawn->pos.y, pawn->pos.x, pawn->symbol);
    wattroff(win, COLOR_PAIR(CP_YELLOW) | A_BOLD);
    return 0;
}

uint8_t pawnMovePath(Pawn *pawn, Map *map, Position target) {
    if (pawn == NULL || map == NULL) return 1;

    // Ensure pawn is inside the map; if not, place at center
    if (!isValidPos(pawn->pos, map)) {
        pawn->pos.x = map->cols / 2;
        pawn->pos.y = map->rows / 2;
    }

    if (!isValidPos(target, map)) return 1;

    if (pawn->pos.x == target.x && pawn->pos.y == target.y) {
        pawn->isPathfinding = false;
        return 0;
    }

    if (target.x != pawn->pathTarget.x || target.y != pawn->pathTarget.y) {
        pawn->pathTarget = target;
        pawn->isPathfinding = true;
    }

    Position next = aStarNextStep(map, pawn->pos, pawn->pathTarget);
    if (next.x == pawn->pos.x && next.y == pawn->pos.y) {
        // no progress possible
        pawn->isPathfinding = false;
        return 0;
    }

    // Move directly to next cell
    pawn->pos = next;
    return 0;
}