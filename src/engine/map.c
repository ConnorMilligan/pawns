#include "engine.h"

#include <stdlib.h>

Map mapBuild(uint16_t rows, uint16_t cols) {
    Map map = {
        .rows = rows,
        .cols = cols,
    };
    map.tiles = malloc(rows * sizeof(Tile*));
    for (size_t i = 0; i < rows; i++) {
        map.tiles[i] = malloc(cols * sizeof(Tile));
        for (size_t j = 0; j < cols; j++) {
            if (rand() % 10 < 1) {
                map.tiles[i][j] = tileBuild(TI_WALL);
            } else {
                map.tiles[i][j] = tileBuild(TI_GRASS);
            }
            map.tiles[i][j].pos.x = j;
            map.tiles[i][j].pos.y = i;
        }
    }
    return map;
}

Tile* mapGetTile(Map *map, Position pos) {
    if (pos.x < 0 || pos.x >= map->cols || pos.y < 0 || pos.y >= map->rows) {
        return NULL;
    }
    return &map->tiles[pos.y][pos.x];
}

uint8_t mapDraw(Map *map, WINDOW *win) {
    if (map == NULL || win == NULL) {
        return 1;
    }

    for (size_t i = 0; i < map->rows; i++) {
        for (size_t j = 0; j < map->cols; j++) {
            if (tileDraw(&map->tiles[i][j], win) != 0) {
                return 1;
            }
        }
    }

    return 0;
}

uint8_t mapDestroy(Map *map) {
    if (map == NULL) {
        return 1;
    }

    for (size_t i = 0; i < map->rows; i++) {
        free(map->tiles[i]);
    }
    free(map->tiles);

    return 0;
}
