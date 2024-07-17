#include "World.h"

World::World() {
    for (int i = 0; i < BORDER; ++i) {
        for (int j = 0; j < BORDER; ++j) {
            map[i][j] = {false, false, false, false, false, false, false}; // Initialize with empty cells
        }
    }
}

CELL World::getCell(int x, int y) const {
    return map[x][y];
}

void World::setCell(int x, int y, const CELL& value) {
    map[x][y] = value;
}
