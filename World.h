#ifndef WORLD_H
#define WORLD_H

#include <vector>

struct CELL {
    bool hasBreeze;
    bool hasStench;
    bool hasPit;
    bool hasWumpus;
    bool hasGlitter;
    bool hasGold;
    bool hasAgent;
    bool hasCreated;
};

#define BORDER 5

class World {
public:
    World();
    CELL getCell(int x, int y) const;
    void setCell(int x, int y, const CELL& value);
    void setCell(int x, int y, char attribute);
private:
    CELL map[BORDER][BORDER];
};

#endif
