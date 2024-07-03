#include <iostream>




struct CELL{
    bool hasBreeze;
    bool hasStench;
    bool hasPit;
    bool hasWumpus;
    bool hasGold;
    bool hasAgent;
};


class World{
    private:
        CELL map[5][5];
    public:
    World(){
        WorldGenerator();
    }
};