#ifndef WORLD_H
#define WORLD_H
#define BORDER 5

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
        CELL map[BORDER][BORDER];

    public:

        World();    
};

#endif 