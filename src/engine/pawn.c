#include "engine.h"
#include <stdlib.h>
#include <math.h>
#include <float.h>

static inline int idx_of(Position p, uint16_t cols) {
    return p.y * cols + p.x;
}

// Check if a position is occupied by another pawn (exclude self)
static uint8_t isPawnOccupied(Position pos, Population *pop, Pawn *self) {
    if (pop == NULL) return 0;
    for (uint16_t i = 0; i < pop->count; i++) {
        if (&pop->pawns[i] == self) continue; // skip self
        if (pop->pawns[i].pos.x == pos.x && pop->pawns[i].pos.y == pos.y) {
            return 1; // occupied
        }
    }
    return 0;
}

// Validate position against map bounds, solidity, and pawn occupancy
static uint8_t isValidPosWithOccupancy(Position pos, Map *map, Population *pop, Pawn *self) {
    if (map == NULL) return 0;
    if (pos.x < 0 || pos.x >= map->cols || pos.y < 0 || pos.y >= map->rows) return 0;
    Tile *t = mapGetTile(map, pos);
    if (t == NULL) return 0;
    if (t->isSolid) return 0;
    // Check pawn occupancy (other pawns block the path)
    if (pop != NULL && isPawnOccupied(pos, pop, self)) return 0;
    return 1;
}

// Compute full path from start to goal, store in pawn's pathCache, avoid other pawns
static uint8_t aStarComputeFullPath(Pawn *pawn, Map *map, Population *pop, Position start, Position goal) {
    if (map == NULL || pawn == NULL) return 1;
    if (!isValidPosWithOccupancy(start, map, pop, pawn) || !isValidPosWithOccupancy(goal, map, pop, pawn)) return 1;

    uint32_t total = (uint32_t)map->rows * (uint32_t)map->cols;
    float *gScore = malloc(total * sizeof(float));
    float *fScore = malloc(total * sizeof(float));
    int *cameFrom = malloc(total * sizeof(int));
    unsigned char *openSet = malloc(total);
    unsigned char *closedSet = malloc(total);
    if (!gScore || !fScore || !cameFrom || !openSet || !closedSet) {
        free(gScore); free(fScore); free(cameFrom); free(openSet); free(closedSet);
        return 1;
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

    while (1) {
        int current = -1;
        float bestF = INFINITY;
        for (uint32_t i = 0; i < total; i++) {
            if (openSet[i] && fScore[i] < bestF) {
                bestF = fScore[i];
                current = (int)i;
            }
        }
        if (current == -1) {
            free(gScore); free(fScore); free(cameFrom); free(openSet); free(closedSet);
            pawn->pathLength = 0;
            pawn->pathIndex = 0;
            return 1; // no path found
        }

        if (current == goalIdx) {
            // reconstruct full path
            int path_stack[256];
            int path_len = 0;
            int cur = goalIdx;
            while (cur != -1 && path_len < 256) {
                path_stack[path_len++] = cur;
                cur = cameFrom[cur];
            }
            // reverse path
            pawn->pathLength = (uint16_t)path_len;
            for (int i = 0; i < path_len; i++) {
                int idx = path_stack[path_len - 1 - i];
                pawn->pathCache[i].x = idx % map->cols;
                pawn->pathCache[i].y = idx / map->cols;
            }
            free(gScore); free(fScore); free(cameFrom); free(openSet); free(closedSet);
            pawn->pathIndex = 0;
            return 0;
        }

        openSet[current] = 0;
        closedSet[current] = 1;

        int cx = current % map->cols;
        int cy = current / map->cols;
        Position neighbors[4] = {{cx+1, cy}, {cx-1, cy}, {cx, cy+1}, {cx, cy-1}};
        for (int ni = 0; ni < 4; ni++) {
            Position np = neighbors[ni];
            if (!isValidPosWithOccupancy(np, map, pop, pawn)) continue;
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
}

Pawn pawnBuild(const char symbol) {
    return (Pawn) {
        .symbol = symbol,
        .pos = {0, 0},
        .pathTarget = {0, 0},
        .isPathfinding = false,
        .pathLength = 0,
        .pathIndex = 0,
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

uint8_t pawnMovePath(Pawn *pawn, Map *map, Population *pop, Position target) {
    if (pawn == NULL || map == NULL) return 1;

    // Ensure pawn is inside the map; if not, place at center
    if (!isValidPosWithOccupancy(pawn->pos, map, pop, pawn)) {
        pawn->pos.x = map->cols / 2;
        pawn->pos.y = map->rows / 2;
    }

    if (!isValidPosWithOccupancy(target, map, pop, pawn)) return 1;

    if (pawn->pos.x == target.x && pawn->pos.y == target.y) {
        pawn->isPathfinding = false;
        return 0;
    }

    // New target? Compute the full path
    if (target.x != pawn->pathTarget.x || target.y != pawn->pathTarget.y) {
        pawn->pathTarget = target;
        aStarComputeFullPath(pawn, map, pop, pawn->pos, target);
        pawn->isPathfinding = (pawn->pathLength > 0);
        if (!pawn->isPathfinding) return 0;
    }

    // Step along cached path only if next position is not occupied
    if (pawn->isPathfinding && pawn->pathIndex < pawn->pathLength) {
        Position nextPos = pawn->pathCache[pawn->pathIndex];
        // Check if next position is occupied (recalculate if blocked)
        if (isPawnOccupied(nextPos, pop, pawn)) {
            pawn->isPathfinding = false; // invalidate path, will recalculate next tick
            return 0;
        }
        pawn->pos = nextPos;
        pawn->pathIndex++;
        if (pawn->pathIndex >= pawn->pathLength) {
            pawn->isPathfinding = false;
        }
    }

    return 0;
}

uint8_t pawnComputePathFull(Pawn *pawn, Map *map, Population *pop, Position goal) {
    if (pawn == NULL || map == NULL) return 1;
    return aStarComputeFullPath(pawn, map, pop, pawn->pos, goal);
}