#include "World.h"
#include "WorldGenerator.h"
#include <iostream>

World::World(){
    WorldGenerator::safePath(this->map);
}