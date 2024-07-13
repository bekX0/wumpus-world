#include "World.h"
#include "WorldGenerator.h"
#include <iostream>

World::World(){
    WorldGenerator::safePath(this->map);
}

CELL World::getCell(int x, int y){
    return this->map[x][y];
}