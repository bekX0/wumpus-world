#include "World.h"
#include <iostream>

World::World() {
    for (int i = 0; i < BORDER; ++i) {
        for (int j = 0; j < BORDER; ++j) {
            map[i][j] = {false, false, false, false, false, false, false, false}; // Initialize with empty cells
        }
    }
}

CELL World::getCell(int x, int y) const {
    return map[x][y];
}

void World::setCell(int x, int y, const CELL& value) {
    if(map[x][y].hasCreated == false){
        map[x][y] = value;
        map[x][y].hasCreated = true;
    }
}

void World::setCell(int x, int y, char attribute) {
    if(y<=BORDER-1 && y>=0 && x>=0 && x<=BORDER-1){
        switch (attribute)
        {
        case 'B':
            map[x][y].hasBreeze = true;
            break;
        case 'S':
            map[x][y].hasStench = true;
            break;
        case 'G':
            map[x][y].hasGlitter = true;
            break;
        
        default:
            std::cout << "Unwritten letter in setCell" << std::endl;
            break;
        }
    }
    else{
        //I crashed to wall dude sorry.
    }
}
