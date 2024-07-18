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
    bool isConsidered;
};

class Agent{
    private:
        AGENTCELL KnownCells[BORDER][BORDER];
        AGENTCELL CurrentCell;
        int health;
        std::pair<int, int> decisionMaker(std::vector<std::pair<AGENTCELL, char>> Cell_and_Direction, int x, int y);
        int decisionScore(AGENTCELL check);
        void predictionProgress(AGENTCELL current, int current_x, int current_y);
        void predictionUpdate(AGENTCELL predicted, int current_x, int current_y);
        AGENTCELL thinkingProgress(AGENTCELL predicted, int predicted_x, int predicted_y);
        std::pair<AGENTCELL, char> consideringProgress(AGENTCELL inferenced, AGENTCELL predicted);
    public:
        friend class World;
        Agent(World world);
        ~Agent();
        void setCurrentCell(AGENTCELL current);
        AGENTCELL CelltoAgentCell(CELL world);
        AGENTCELL getCurrentCell();
        AGENTCELL getKnownCell(int x, int y);
        void movement(int x, int y, World world);
        void updateKnowledge(AGENTCELL Learned, int x, int y);
};
#endif