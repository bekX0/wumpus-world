#ifndef AGENT_H
#define AGENT_H
#include "World.h"
#include "WorldGenerator.h"
#include <vector>
#define BORDER 5

struct AGENTCELL{
    bool hasBreeze;
    bool hasStench;
    bool hasGlitter;
    int hasPit;
    int hasWumpus;
    int hasGold;
    bool isVisited;
    bool isSafe;
    bool isRecentlyVisited;
};

class Agent{
    private:
        AGENTCELL KnownCells[BORDER][BORDER];
        AGENTCELL CurrentCell;
        std::pair<int, int> decisionMaker(std::vector<std::pair<AGENTCELL, char>> Cell_and_Direction, int x, int y);
        int decisionScore(AGENTCELL check);
        void predictionProgress(AGENTCELL current, int x, int y);
        void predictionUpdate(AGENTCELL predicted, int x, int y);
    public:
        Agent();
        ~Agent();
        void setCurrentCell(AGENTCELL current, int x, int y);
        AGENTCELL getCurrentCell();
        AGENTCELL getKnownCell(int x, int y);
        void movement(int x, int y);
        void updateKnowledge(AGENTCELL Learned, int x, int y);
};
#endif