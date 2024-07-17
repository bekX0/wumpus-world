#include <iostream>
#include "World.h"
#include "WorldGenerator.h"

void printWorld(const World& world) {
    for (int i = 0; i < BORDER; ++i) {
        for (int j = 0; j < BORDER; ++j) {
            CELL cell = world.getCell(i, j);
            std::cout << "[";
            if (cell.hasGold) std::cout << "G";
            else if (cell.hasWumpus) std::cout << "W";
            else if (cell.hasPit) std::cout << "P";
            else std::cout << " ";
            std::cout << "]";
        }
        std::cout << std::endl;
    }
}

int main() {
    World world = WorldGenerator::newWorld();
    printWorld(world);

    return 0;
}
