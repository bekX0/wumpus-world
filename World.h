#ifndef WORLD_H
#define WORLD_H

#include <vector>
enum CELL {
    EMPTY,
    GOLD,
    WUMPUS,
    PIT
};

#define BORDER 5


class World {
public:
    World();
    CELL getCell(int x, int y);
    void setCell(int x, int y, CELL value);
    void display(); // Add this function

private:
    CELL map[BORDER][BORDER];
};

#endif
