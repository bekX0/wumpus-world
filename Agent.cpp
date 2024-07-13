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
    AGENTCELL real_like_prediction = predicted; //if first prediction is correct
        if(y<BORDER-1) //if the Agent is not at LAST column
        {
            if(KnownCells[x-1][y].isSafe==false)
            {
                real_like_prediction = thinkingProgress(predicted, x, y+1); //consider predicted again by adjacent AGENTCELLS of it
                updateKnowledge(real_like_prediction, x, y+1); //predict right AGENTCELL
            }
        }
        
        if(y>0) //if the Agent is not at FIRST column
        {
            if(KnownCells[x-1][y].isSafe==false)
            {
                real_like_prediction = thinkingProgress(predicted, x, y-1); //consider predicted again by adjacent AGENTCELLS of it
                updateKnowledge(predicted, x, y-1); //predict left AGENTCELL
            }
        }

        if(x>0) //if the Agent is not at FIRST row
        {
            if(KnownCells[x-1][y].isSafe==false)
            {
                real_like_prediction = thinkingProgress(predicted, x-1, y); //consider predicted again by adjacent AGENTCELLS of it
                updateKnowledge(predicted, x-1, y); //predict above AGENTCELL
            }
        }
        
        if(x<BORDER-1) //if the Agent if not at LAST Row
        {
            if(KnownCells[x-1][y].isSafe==false)
            {
                real_like_prediction = thinkingProgress(predicted, x+1, y); //consider predicted again by adjacent AGENTCELLS of it
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

std::pair<AGENTCELL, char> Agent::consideringProgress(AGENTCELL inferenced, AGENTCELL predicted)
{
    //Commented codelines say that we can't exactly predict there must be a pit or wumpus by looking adjacent cells and finds there is breeze or stench
    //Open codelines say that we can exactly predict there can't be a pit or wumpus by looking adjacent cells and finds there is no breeze or stench

    if(inferenced.hasBreeze == false && inferenced.hasStench == false) //if there is no breeze and stench in inferenced cell
    {
        if(predicted.hasPit >= 0) //and predicted cell considired as may hasPit
        predicted.hasPit = -1; //statement redesigned as there is no Pit

        if(predicted.hasWumpus >= 0) //and predicted cell considired as may hasWumpus
        predicted.hasWumpus = -1; //statement redesigned as there is no Wumpus
 
        return std::make_pair(predicted, 'C'); //returns new predicted and 'C' for "Changed"
    }
    if(inferenced.hasBreeze == false && inferenced.hasStench == true) //if there is no breeze but stench in inferenced cell
    {
        if(predicted.hasWumpus <= 0) //and predicted cell considired as may not hasWumpus
        predicted.hasWumpus = 0; //statement redesigned as there is may Wumpus

        // if(predicted.hasPit >= 0) //and predicted cell considired as may hasPit
        // predicted.hasPit = -1; //statement redesigned as there is no Pit

        return std::make_pair(predicted, 'C'); //returns new predicted and 'C' for "Changed"
    }
    if(inferenced.hasBreeze == true && inferenced.hasStench == false) //if there is no stench but breeze in inferenced cell
    {
        if(predicted.hasPit <= 0) //and predicted cell considired as may not hasPit
        predicted.hasPit = 0; //statement redesigned as there is may Pit

        // if(predicted.hasWumpus >= 0) //and predicted cell considired as may hasWumpus
        // predicted.hasWumpus = -1; //statement redesigned as there is no Wumpus

        return std::make_pair(predicted, 'C'); //returns new predicted and 'C' for "Changed"
    }
    if(inferenced.hasBreeze == true && inferenced.hasStench == true) //if there is no stench but breeze in inferenced cell
    {
        if(predicted.hasPit <= 0) //and predicted cell considired as may not hasPit
        predicted.hasPit = 0; //statement redesigned as there is may Pit

        if(predicted.hasWumpus >= 0) //and predicted cell considired as may hasWumpus
        predicted.hasWumpus = -1; //statement redesigned as there is no Wumpus

        return std::make_pair(predicted, 'C'); //returns new predicted and 'C' for "Changed"
    }

    return std::make_pair(predicted, 'N'); //returns predicted as taken and 'N' for "Not Changed"
}

AGENTCELL Agent::thinkingProgress(AGENTCELL predicted, int x, int y)
{
    //Known issue is if adjacent cells of real_like_prediction has breeze it will marked as there is a pit exactly
    //But this case is not true all the time

    std::pair<AGENTCELL, char> cell_and_mark;
    AGENTCELL real_like_prediction = predicted; //if none of any statement is true
    AGENTCELL inferenced_cell;

        if(y<BORDER-2 && KnownCells[x][y+1].isVisited==true && KnownCells[x][y+1].isRecentlyVisited==false) //if the right of the predicted AGENTCELL is not at LAST column and already visited and not recently visited
        {
            inferenced_cell = getKnownCell(x, y+1); //right of the predicted in this case
            cell_and_mark = consideringProgress(inferenced_cell, predicted);
            if(cell_and_mark.second=='C')
            {
                real_like_prediction = cell_and_mark.first;
                return real_like_prediction;
            }
        }
        
        if(y>1 && KnownCells[x][y-1].isVisited==true && KnownCells[x][y-1].isRecentlyVisited==false) //if the left of the predicted AGENTCELL is not at LAST column and already visited and not recently visited
        {
            inferenced_cell = getKnownCell(x, y-1); //left of the predicted in this case
            cell_and_mark = consideringProgress(inferenced_cell, predicted);
            if(cell_and_mark.second=='C')
            {
                real_like_prediction = cell_and_mark.first;
                return real_like_prediction;
            }
        }

        if(x>1 && KnownCells[x-1][y].isVisited==true && KnownCells[x-1][y].isRecentlyVisited==false) //if the above of the predicted AGENTCELL is not at LAST column and already visited and not recently visited
        {
            inferenced_cell = getKnownCell(x-1, y); //above of the predicted in this case
            cell_and_mark = consideringProgress(inferenced_cell, predicted);
            if(cell_and_mark.second=='C')
            {
                real_like_prediction = cell_and_mark.first;
                return real_like_prediction;
            }
        }
        
        if(x<BORDER-2 && KnownCells[x+1][y].isVisited==true && KnownCells[x+1][y].isRecentlyVisited==false) //if the below of the predicted AGENTCELL is not at LAST column and already visited and not recently visited
        {
            inferenced_cell = getKnownCell(x+1, y); //below of the predicted in this case
            cell_and_mark = consideringProgress(inferenced_cell, predicted);
            if(cell_and_mark.second=='C')
            {
                real_like_prediction = cell_and_mark.first;
                return real_like_prediction;
            }
        }
    return real_like_prediction;
}

void Agent::movement(int x, int y)
{   
    std::pair<int, int> coordinates;
    std::vector<std::pair<int, int>> path;
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
            cout << "I am at x: " << x << " y: " << y << endl;
            path.emplace_back(x,y); //Current CELL before moving
            KnownCells[x][y].isRecentlyVisited=true; //Sets current CELL as RecentlyVisited 
            coordinates = decisionMaker(Cell_and_Direction, x, y); //Decides upcoming CELL's coordinates
            x = coordinates.first;
            y = coordinates.second;
            choosen_cell = World::getCell(coordinates.first, coordinates.second); //Gets real values of CELL from World
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
            path.emplace_back(x,y); //Current CELL after moving
            if(path.size()>2)
            {
                KnownCells[path[path.size()-2].first][path[path.size()-2].second].isRecentlyVisited = false; //Sets CELL that 2 steps behind the current in PATH as not RecentlyVisited
            }
            cout << "I moved to x: " << x << " y: " << y << endl;
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