#include <iostream>
#include "WorldGenerator.h"
#include "World.h"
#include "Agent.h"
#include <stack>
#include <vector>
#include <cstdlib>
#include <time.h>

#define BORDER 5

std::pair<int, int> Agent::decisionMaker(std::vector<std::pair<AGENTCELL, char>> Cell_and_Direction, int x, int y)
{
    AGENTCELL cell;
    int score = 0;
    int max = 0;
    char direction = 'R';
    std::vector<std::pair<AGENTCELL, int>> Cell_and_Score;

    for(int i=0; i<Cell_and_Direction.size(); i++)
    {
        cell = Cell_and_Direction[0].first;
        score = decisionScore(cell);
        Cell_and_Score.emplace_back(cell, score);
    }

    max = Cell_and_Score[0].second;
    cell = Cell_and_Score[0].first;

    for(int i=0; i<Cell_and_Score.size()-1; i++)
    {
        if(Cell_and_Score[i].second > Cell_and_Score[i+1].second)
        {
            max = Cell_and_Score[i].second;
            cell = Cell_and_Score[i].first;
            direction = Cell_and_Direction[i].second;
        }
    }

    switch (direction)
    {
    case 'R':
        x = x;
        y = y+1;
        break;
    case 'L':
        x = x;
        y = y-1;
        break;
    case 'A':
        x = x-1;
        y = y;
        break;
    case 'B':
        x = x+1;
        y = y;
        break;
    default:
        break;
    }
    
    return std::make_pair(x,y);
}

int Agent::decisionScore(AGENTCELL check)
{
    int total = 0;
    if(check.hasGold == 1)
    {
        total = total + 10000;
    }
    if(check.hasPit == 1)
    {
        total = total - 20000;
    }
    if(check.hasWumpus == 1)
    {
        total = total - 20000;
    }
    if(check.hasGold == 0)
    {
        total = total + 100;
    }
    if(check.hasPit == 0)
    {
        total = total - -500;
    }
    if(check.hasWumpus == 0)
    {
        total = total - -500;
    }
    if(check.hasPit == -1 && check.hasPit == -1)
    {
        total = total + 50;
    }
    if(check.hasWumpus == -1  && check.hasPit == -1)
    {
        total = total + 50;
    }
    if(check.isSafe == true && check.isVisited == false && check.isRecentlyVisited == false)
    {
        total = total + 100;
        if(check.hasGlitter == true)
        {
            total = total + 50;
        }
    }
    if(check.isSafe == true && check.isVisited == true && check.isRecentlyVisited == true)
    {
        total = total - 75;
        if(check.hasGlitter == true)
        {
            total = total + 50;
        }
    }
    return total;
}

void Agent::predictionUpdate(AGENTCELL predicted, int x, int y)
{
        if(y<BORDER-1) //if the Agent is not at LAST column
        {
            if(KnownCells[x-1][y].isSafe==false)
            {
                updateKnowledge(predicted, x, y+1); //predict right AGENTCELL
            }
        }
        
        if(y>0) //if the Agent is not at FIRST column
        {
            if(KnownCells[x-1][y].isSafe==false)
            {
                updateKnowledge(predicted, x, y-1); //predict left AGENTCELL
            }
        }

        if(x>0) //if the Agent is not at FIRST row
        {
            if(KnownCells[x-1][y].isSafe==false)
            {
                updateKnowledge(predicted, x-1, y); //predict above AGENTCELL
            }
        }
        
        if(x<BORDER-1) //if the Agent if not at LAST Row
        {
            if(KnownCells[x-1][y].isSafe==false)
            {
                updateKnowledge(predicted, x+1, y); //predict below AGENTCELL
            }
        }
        
}

void Agent::predictionProgress(AGENTCELL current, int x, int y)
{
    if(current.hasBreeze == false && current.hasStench == false && current.hasGlitter == true)
    {
        AGENTCELL predicted = AGENTCELL
        {
            false,
            false,
            false,
            -1,
            -1,
            0,
            false,
            true,
        };
        predictionUpdate(predicted, x, y);
    }
    else if(current.hasBreeze == true && current.hasStench == false && current.hasGlitter == true)
    {
        AGENTCELL predicted = AGENTCELL
        {
            false,
            false,
            false,
            0,
            -1,
            0,
            false,
            false,
        };
        predictionUpdate(predicted, x, y);
    }
    else if(current.hasBreeze == false && current.hasStench == true && current.hasGlitter == true)
    {
        AGENTCELL predicted = AGENTCELL
        {
            false,
            false,
            false,
            -1,
            0,
            0,
            false,
            false,
        };
        predictionUpdate(predicted, x, y);
    }
    else if(current.hasBreeze == true && current.hasStench == true & current.hasGlitter == true)
    {
        AGENTCELL predicted = AGENTCELL
        {
            false,
            false,
            false,
            0,
            0,
            0,
            false,
            false,
        };
        predictionUpdate(predicted, x, y);
    }
    else if(current.hasBreeze == false && current.hasStench == false && current.hasGlitter == false)
    {
        AGENTCELL predicted = AGENTCELL
        {
            false,
            false,
            false,
            -1,
            -1,
            -1,
            false,
            true,
        };
        predictionUpdate(predicted, x, y);
    }
    else if(current.hasBreeze == true && current.hasStench == false && current.hasGlitter == false)
    {
        AGENTCELL predicted = AGENTCELL
        {
            false,
            false,
            false,
            0,
            -1,
            -1,
            false,
            false,
        };
        predictionUpdate(predicted, x, y);
    }
    else if(current.hasBreeze == false && current.hasStench == true && current.hasGlitter == false)
    {
        AGENTCELL predicted = AGENTCELL
        {
            false,
            false,
            false,
            -1,
            0,
            0,
            false,
            false,
        };
        predictionUpdate(predicted, x, y);
    }
    else if(current.hasBreeze == true && current.hasStench == true & current.hasGlitter == false)
    {
        AGENTCELL predicted = AGENTCELL
        {
            false,
            false,
            false,
            0,
            0,
            -1,
            false,
            false,
        };
        predictionUpdate(predicted, x, y);
    }
}

void Agent::movement(int x, int y)
{   
    std::pair<int, int> coordinates;
    CELL choosen_cell;
    while(!getCurrentCell().hasGold)
    {   
        predictionProgress(CurrentCell, x, y);
        std::vector<std::pair<AGENTCELL, char>> Cell_and_Direction;
        for(int i=0; i<4; i++)
        {
            switch (i)
            {
            case 0:
                if(y<BORDER-1)
                {
                    Cell_and_Direction.emplace_back(getKnownCell(x, y+1), 'R'); //Right
                }
                break;
            case 1:
                if(y>0)
                {
                    Cell_and_Direction.emplace_back(getKnownCell(x, y-1), 'L'); //Left
                }
                break;
            case 2:
                if(x>0)
                {
                    Cell_and_Direction.emplace_back(getKnownCell(x-1, y), 'A'); //Above
                }
                break;
            case 3:
                if(x<BORDER-1)
                {
                    Cell_and_Direction.emplace_back(getKnownCell(x+1, y), 'B'); //Below
                }
                break;
        
            default:
                std::cout << "Something wrong with movement" << endl;
                break;
            }
            KnownCells[x][y].isRecentlyVisited=true;
            coordinates = decisionMaker(Cell_and_Direction, x, y);
            choosen_cell = World::getCell(coordinates.first, coordinates.second);
            AGENTCELL current =
            {
                choosen_cell.hasBreeze,
                choosen_cell.hasStench,
                choosen_cell.hasGlitter,
                choosen_cell.hasPit,
                choosen_cell.hasWumpus,
                choosen_cell.hasGold,
                true,
                true,
                true
            };
            setCurrentCell(current, x, y);
        }
    }
    cout << "I found gold at x: " << x << " y: " << y << endl;
}

void Agent::updateKnowledge(AGENTCELL learned, int x, int y)
{
    KnownCells[x][y] = learned;
}

Agent::Agent()
{
    CELL start = World::getCell(0, 0);
    AGENTCELL current =
    {
        start.hasBreeze,
        start.hasStench,
        start.hasGlitter,
        0,
        0,
        0,
        true,
        true
    }; 
    setCurrentCell(current, 0, 0);
    updateKnowledge(current, 0, 0);
    movement(0, 0);
}