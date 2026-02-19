#include "engine.h"

#include <stdlib.h>

Tile tileBuild(enum TileID id) {
    switch (id) {
        case TI_GRASS:
            // random symbols for grass
            char symbols[] = {',', '.', '`', '\"'};
            return (Tile) {
                .symbol = symbols[rand() % 4],
                .name = "Grass",
                .color = CP_GREEN,
                .isSolid = false,
                .pos = {0, 0},
            };
        case TI_WALL:
            return (Tile) {
                .symbol = '#',
                .name = "Wall",
                .color = CP_WHITE,
                .isSolid = true,
                .pos = {0, 0},
            };
        default:
            return (Tile) {
                .symbol = '?',
                .name = "Unknown",
                .color = CP_RED,
                .isSolid = true,
                .pos = {0, 0},
            };
    }

}

uint8_t tileDraw(Tile *tile, WINDOW *win) {
    if (tile == NULL || win == NULL) {
        return 1;
    }

    wattron(win, COLOR_PAIR(tile->color));
    mvwaddch(win, tile->pos.y, tile->pos.x, tile->symbol);
    wattroff(win, COLOR_PAIR(tile->color));

    return 0;
}