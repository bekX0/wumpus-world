#include "World.h"
#include <iostream>

World::World() {
    for (int i = 0; i < BORDER; ++i) {
        for (int j = 0; j < BORDER; ++j) {
            map[i][j] = EMPTY; // Initialize with empty cells
        }
    }
}

CELL World::getCell(int x, int y) {
    return map[x][y];
}

void World::setCell(int x, int y, CELL value) {
    map[x][y] = value;
}

void World::display() {
    for (int i = 0; i < BORDER; ++i) {
        for (int j = 0; j < BORDER; ++j) {
            switch (map[i][j]) {
                case EMPTY:
                    std::cout << ".";
                    break;
                case GOLD:
                    std::cout << "G";
                    break;
                case WUMPUS:
                    std::cout << "W";
                    break;
                case PIT:
                    std::cout << "P";
                    break;
                default:
                    std::cout << "?";
                    break;
            }
        }
        std::cout << std::endl;
    }
}
