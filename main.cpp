#include <iostream>
#include "World.h"
#include "WorldGenerator.h"
#include "Agent.h"

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
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    for (int i = 0; i < BORDER; ++i) {
        for (int j = 0; j < BORDER; ++j) {
            CELL cell = world.getCell(i, j);
            std::cout << "[";
            if (cell.hasStench) std::cout << "S";
            if (cell.hasBreeze) std::cout << "B";
            if (cell.hasGlitter) std::cout << "G";
            else std::cout << " ";
            std::cout << "]";
        }
        std::cout << std::endl;
    }
}

int main() {
    World world = WorldGenerator::newWorld();
    printWorld(world);
    Agent agent(world);
    //get playyyyy

    return 0;
}
