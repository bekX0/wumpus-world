#include <iostream>
#include "World.h"
#include "WorldGenerator.h"

int main() {
    World world = WorldGenerator::newWorld();
    world.display();
    return 0;
}
