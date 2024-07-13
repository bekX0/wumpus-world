#ifndef WORLD_H
#define WORLD_H
#define BORDER 5

struct CELL{
    bool hasBreeze;
    bool hasStench;
    bool hasPit;
    bool hasWumpus;
    bool hasGlitter;
    bool hasGold;
    bool hasAgent;
}; 


class World{

    private:
        CELL map[BORDER][BORDER];
        CELL getCell(int x, int y);
    public:

        World();    
};

#endif 