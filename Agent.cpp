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
    int score;
    int max;
    int coordinate_x = x;
    int coordinate_y = y;
    char direction;
    int index_rand;
    int index_max;
    //int last_chance;
    std::vector<std::pair<AGENTCELL, int>> Cell_and_Score;
    std::vector<int> indexof_maxs;
    srand(time(0));
    while(true)
    {
        max = -1000000;
        score = -1000000;
        Cell_and_Score.clear();
        indexof_maxs.clear();
        for(int i=0; i<Cell_and_Direction.size(); ++i)
        {
            cell = Cell_and_Direction[i].first;
            score = decisionScore(cell);
            Cell_and_Score.emplace_back(cell, score);
            std::cout << "I can move the direction: " << Cell_and_Direction[i].second << " Index of direction: " << i << std::endl << std::endl;
        }

        for(int l=0; l<Cell_and_Score.size(); l++)
        {
            std::cout << "Score of cell: " << Cell_and_Direction[l].second << "  " << Cell_and_Score[l].second << " Index of Cell: " << l << std::endl << std::endl;
            if(Cell_and_Score[l].second > max)
            {
                max = Cell_and_Score[l].second;
                cell = Cell_and_Score[l].first;
                direction = Cell_and_Direction[l].second;
                index_max = l;
                //std::cout << " Index of MAX: " << index_max << " and index:" << l << std::endl;
                indexof_maxs.clear();
                indexof_maxs.emplace_back(l);
            }
            else if(Cell_and_Score[l].second == max)
            {
                indexof_maxs.emplace_back(l);
            }
        }
        if(indexof_maxs.size()>1)
        {
            for(int j=0; j<indexof_maxs.size(); j++) //check cells that has same max score
            {
                if(Cell_and_Score[indexof_maxs[j]].first.isRecentlyVisited == true) //if there is a cell that is recently visited
                {
                    indexof_maxs.erase(indexof_maxs.begin() + j); //delete it to prevent loop
                }
            }
            index_rand = rand() % indexof_maxs.size(); //chooses an index of max from vector of same max scores
            index_max = indexof_maxs[index_rand]; //get this index from the vector
            max = Cell_and_Score[index_max].second;
            cell = Cell_and_Score[index_max].first;
            direction = Cell_and_Direction[index_max].second;
            //std::cout << " Index of MAX: " << index_max << " and index random:" << index_rand << std::endl;
        }

        if(cell.isRecentlyVisited == true && stress > 0) //if Agent deciede to go a Cell that RecentlyVisited
        {
            stress = stress - 1; //Agent's stress level goes down
            std::cout << "My stress level now: " << stress << std::endl << std::endl;
            break;
        }
        else if(cell.isRecentlyVisited == true && stress <= 0) //if Agent deciede to go a Cell that RecentlyVisited but don't have stress
        {
            for(int k=0; k<Cell_and_Direction.size(); ++k)
            {
                if(Cell_and_Direction[k].first.hasPit <= 0 && Cell_and_Direction[k].first.hasWumpus <= 0 && k != index_max) //if there is another Cell that has no Pit and Wumpus and is not the decieded one
                {   
                    stress = 2;
                    std::cout << "I choosed the direction: " << direction << " with point: " << max << std::endl << std::endl;
                    std::cout << "Let's think again!" << std::endl << std::endl;
                    Cell_and_Direction.erase(Cell_and_Direction.begin() + index_max);
                    //std::cout << " Index of MAX: " << index_max << " k is: " << k << std::endl;
                    break;
                }
            }
            if(stress != 2)
            {
                stress = 0;
                std::cout << "Can't choose a better way other than going back!" << std::endl << std::endl;
                break;
            }
            // else
            // {
                
            //     continue;
            // }
        }
        else
        {
            break;
        }
    }
    

    switch (direction)
    {
    case 'R':
        coordinate_x = coordinate_x;
        coordinate_y = coordinate_y+1;
        break;
    case 'L':
        coordinate_x = coordinate_x;
        coordinate_y = coordinate_y-1;
        break;
    case 'A':
        coordinate_x = coordinate_x-1;
        coordinate_y = coordinate_y;
        break;
    case 'B':
        coordinate_x = coordinate_x+1;
        coordinate_y = coordinate_y;
        break;
    default:
        break;
    }
    std::cout << "I choosed the direction: " << direction << " with point: " << max << std::endl << std::endl;
    return std::make_pair(coordinate_x,coordinate_y);
}

int Agent::decisionScore(AGENTCELL check)
{
    int total = 0;
    if(check.hasGold == 1)
    {
        total = total + 100000;
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
        total = total - 1000;
    }
    if(check.hasWumpus == 0)
    {
        total = total - 1000;
    }
    if(check.hasPit == -1 && check.hasPit == -1)
    {
        total = total + 50;
    }
    if(check.isSafe == true && check.isVisited == false && check.isRecentlyVisited == false)
    {
        total = total + 500;
        if(check.hasGlitter == true)
        {
            total = total + 100;
        }
    }
    if(check.isSafe == true && check.isVisited == true && check.isRecentlyVisited == false)
    {
        total = total - 75;
        if(check.hasGlitter == true)
        {
            total = total + 100;
        }
    }
    if(check.isSafe == true && check.isVisited == true && check.isRecentlyVisited == true)
    {
        total = total - 300;
        if(check.hasGlitter == true)
        {
            total = total + 100;
        }
    }
    return total;
}

void Agent::predictionUpdate(AGENTCELL predicted, int current_x, int current_y)
{
    AGENTCELL real_like_prediction = predicted; //if first prediction is correct
        if(current_y < BORDER-1) //if the Agent is not at LAST column
        {
            if(KnownCells[current_x][current_y+1].isVisited==false)
            {
                if(KnownCells[current_x][current_y+1].isConsidered==false)
                real_like_prediction = thinkingProgress(predicted, current_x, current_y+1); //consider predicted again by adjacent AGENTCELLS of it

                updateKnowledge(real_like_prediction, current_x, current_y+1); //predict right AGENTCELL
            }
        }
        
        if(current_y > 0) //if the Agent is not at FIRST column
        {
            if(KnownCells[current_x][current_y-1].isVisited==false)
            {
                if(KnownCells[current_x][current_y-11].isConsidered==false)
                real_like_prediction = thinkingProgress(predicted, current_x, current_y-1); //consider predicted again by adjacent AGENTCELLS of it

                updateKnowledge(predicted, current_x, current_y-1); //predict left AGENTCELL
            }
        }

        if(current_x > 0) //if the Agent is not at FIRST row
        {
            if(KnownCells[current_x-1][current_y].isVisited==false)
            {
                if(KnownCells[current_x-1][current_y].isConsidered==false)
                real_like_prediction = thinkingProgress(predicted, current_x-1, current_y); //consider predicted again by adjacent AGENTCELLS of it

                updateKnowledge(predicted, current_x-1, current_y); //predict above AGENTCELL
            }
        }
        
        if(current_x < BORDER-1) //if the Agent if not at LAST Row
        {
            if(KnownCells[current_x+1][current_y].isVisited==false)
            {
                if(KnownCells[current_x+1][current_y].isConsidered==false)
                real_like_prediction = thinkingProgress(predicted, current_x+1, current_y); //consider predicted again by adjacent AGENTCELLS of it

                updateKnowledge(predicted, current_x+1, current_y); //predict below AGENTCELL
            }
        }
        
}

void Agent::predictionProgress(AGENTCELL current, int current_x, int current_y)
{
    AGENTCELL predicted =
    {
        true,
        true,
        false,
        0,
        0,
        -1,
        false,
        false,
        false,
        false
    }; //Initial prediction based on caution.

    if(current.hasBreeze == false)
    {
        predicted.hasPit = -1;
    }
    if(current.hasStench == false)
    {
        predicted.hasWumpus = -1;
    }
    if(current.hasGlitter == true)
    {
        predicted.hasGold = 0;
        predicted.hasGlitter = 0;
    }

    predictionUpdate(predicted, current_x, current_y);

    // if(current.hasBreeze == false && current.hasStench == false && current.hasGlitter == true)
    // {
    //     AGENTCELL predicted = AGENTCELL
    //     {
    //         false,
    //         false,
    //         false,
    //         -1,
    //         -1,
    //         0,
    //         false,
    //         true,
    //         false
    //     };
    //     predictionUpdate(predicted, x, y);
    // }
    // else if(current.hasBreeze == true && current.hasStench == false && current.hasGlitter == true)
    // {
    //     AGENTCELL predicted = AGENTCELL
    //     {
    //         false,
    //         false,
    //         false,
    //         0,
    //         -1,
    //         0,
    //         false,
    //         false,
    //         false
    //     };
    //     predictionUpdate(predicted, x, y);
    // }
    // else if(current.hasBreeze == false && current.hasStench == true && current.hasGlitter == true)
    // {
    //     AGENTCELL predicted = AGENTCELL
    //     {
    //         false,
    //         false,
    //         false,
    //         -1,
    //         0,
    //         0,
    //         false,
    //         false,
    //         false
    //     };
    //     predictionUpdate(predicted, x, y);
    // }
    // else if(current.hasBreeze == true && current.hasStench == true & current.hasGlitter == true)
    // {
    //     AGENTCELL predicted = AGENTCELL
    //     {
    //         false,
    //         false,
    //         false,
    //         0,
    //         0,
    //         0,
    //         false,
    //         false,
    //         false
    //     };
    //     predictionUpdate(predicted, x, y);
    // }
    // else if(current.hasBreeze == false && current.hasStench == false && current.hasGlitter == false)
    // {
    //     AGENTCELL predicted = AGENTCELL
    //     {
    //         false,
    //         false,
    //         false,
    //         -1,
    //         -1,
    //         -1,
    //         false,
    //         true,
    //         false
    //     };
    //     predictionUpdate(predicted, x, y);
    // }
    // else if(current.hasBreeze == true && current.hasStench == false && current.hasGlitter == false)
    // {
    //     AGENTCELL predicted = AGENTCELL
    //     {
    //         false,
    //         false,
    //         false,
    //         0,
    //         -1,
    //         -1,
    //         false,
    //         false,
    //         false
    //     };
    //     predictionUpdate(predicted, x, y);
    // }
    // else if(current.hasBreeze == false && current.hasStench == true && current.hasGlitter == false)
    // {
    //     AGENTCELL predicted = AGENTCELL
    //     {
    //         false,
    //         false,
    //         false,
    //         -1,
    //         0,
    //         0,
    //         false,
    //         false,
    //         false
    //     };
    //     predictionUpdate(predicted, x, y);
    // }
    // else if(current.hasBreeze == true && current.hasStench == true & current.hasGlitter == false)
    // {
    //     AGENTCELL predicted = AGENTCELL
    //     {
    //         false,
    //         false,
    //         false,
    //         0,
    //         0,
    //         -1,
    //         false,
    //         false,
    //         false
    //     };
    //     predictionUpdate(predicted, x, y);
    // }
}

std::pair<AGENTCELL, char> Agent::consideringProgress(AGENTCELL inferenced, AGENTCELL predicted)
{
    //Commented codelines say that we can't exactly predict there must be a pit or wumpus by looking adjacent cells and finds there is breeze or stench//Statemnet Changed
    //Open codelines say that we can exactly predict there can't be a pit or wumpus by looking adjacent cells and finds there is no breeze or stench//Statement Changed

    std::pair<AGENTCELL, char> cell_and_mark = std::make_pair(predicted, 'N'); //saves predicted as taken and 'N' for "Not Changed"

    if(inferenced.hasBreeze == false && predicted.hasPit > -1) //if there is no breeze in inferenced cell and predicted cell considired as may hasPit
    {
        predicted.hasPit = -1; //statement redesigned as there is no Pit
        predicted.isConsidered = true;
        cell_and_mark = std::make_pair(predicted, 'C'); //modified predicted and 'C' for "Changed"
    }
    if(inferenced.hasStench == false && predicted.hasWumpus > -1) //if there is no stench in inferenced cell and predicted cell considired as may hasWumpus
    {
        predicted.hasWumpus = -1; //statement redesigned as there is no Wumpus
        predicted.isConsidered = true;
        cell_and_mark = std::make_pair(predicted, 'C'); //modified predicted and 'C' for "Changed"
    }
    if(inferenced.hasBreeze == false && inferenced.hasStench == false && predicted.isSafe == false)
    {
        predicted.isSafe == true;
        predicted.isConsidered = true;
        cell_and_mark = std::make_pair(predicted, 'C'); //modified predicted and 'C' for "Changed"
    }
    if(inferenced.hasGlitter == false && predicted.hasGold > -1) //if there is no glitter in inferenced cell and predicted cell considired as may hasGold
    {
        predicted.hasGold = -1; //statement redesigned as there is no Gold
        predicted.isConsidered = true;
        cell_and_mark = std::make_pair(predicted, 'C'); //modified predicted and 'C' for "Changed"
    }
    // if(inferenced.hasBreeze == false && inferenced.hasStench == false) //if there is no breeze and stench in inferenced cell
    // {
    //     if(predicted.hasPit >= 0) //and predicted cell considired as may hasPit
    //     predicted.hasPit = -1; //statement redesigned as there is no Pit

    //     if(predicted.hasWumpus >= 0) //and predicted cell considired as may hasWumpus
    //     predicted.hasWumpus = -1; //statement redesigned as there is no Wumpus
 
    //     return std::make_pair(predicted, 'C'); //returns new predicted and 'C' for "Changed"
    // }
    // if(inferenced.hasBreeze == false && inferenced.hasStench == true) //if there is no breeze but stench in inferenced cell
    // {
    //     if(predicted.hasWumpus <= 0) //and predicted cell considired as may not hasWumpus
    //     predicted.hasWumpus = 0; //statement redesigned as there is may Wumpus

    //     // if(predicted.hasPit >= 0) //and predicted cell considired as may hasPit
    //     // predicted.hasPit = -1; //statement redesigned as there is no Pit

    //     return std::make_pair(predicted, 'C'); //returns new predicted and 'C' for "Changed"
    // }
    // if(inferenced.hasBreeze == true && inferenced.hasStench == false) //if there is no stench but breeze in inferenced cell
    // {
    //     if(predicted.hasPit <= 0) //and predicted cell considired as may not hasPit
    //     predicted.hasPit = 0; //statement redesigned as there is may Pit

    //     // if(predicted.hasWumpus >= 0) //and predicted cell considired as may hasWumpus
    //     // predicted.hasWumpus = -1; //statement redesigned as there is no Wumpus

    //     return std::make_pair(predicted, 'C'); //returns new predicted and 'C' for "Changed"
    // }
    // if(inferenced.hasBreeze == true && inferenced.hasStench == true) //if there is no stench but breeze in inferenced cell
    // {
    //     if(predicted.hasPit <= 0) //and predicted cell considired as may not hasPit
    //     predicted.hasPit = 0; //statement redesigned as there is may Pit

    //     if(predicted.hasWumpus >= 0) //and predicted cell considired as may hasWumpus
    //     predicted.hasWumpus = -1; //statement redesigned as there is no Wumpus

    //     return std::make_pair(predicted, 'C'); //returns new predicted and 'C' for "Changed"
    // }

    return cell_and_mark;
}

AGENTCELL Agent::thinkingProgress(AGENTCELL predicted, int predicted_x, int predicted_y)
{
    //Known issue is if adjacent cells of real_like_prediction has breeze it will marked as there is a pit exactly
    //But this case is not true all the time

    std::pair<AGENTCELL, char> cell_and_mark; //'C' for Changed 'N' for Not Changed/Unchanged
    AGENTCELL real_like_prediction = predicted; //if none of any statement is true
    AGENTCELL inferenced_cell;

        if(predicted_y<BORDER-1 && KnownCells[predicted_x][predicted_y+1].isVisited==true && KnownCells[predicted_x][predicted_y+1].isRecentlyVisited==false) //if the right of the predicted AGENTCELL is not at LAST column and already visited and not recently visited
        {
            inferenced_cell = getKnownCell(predicted_x, predicted_y+1); //right of the predicted in this case
            cell_and_mark = consideringProgress(inferenced_cell, predicted);
            if(cell_and_mark.second=='C')
            {
                real_like_prediction = cell_and_mark.first;
                return real_like_prediction;
            }
        }
        
        if(predicted_y>0 && KnownCells[predicted_x][predicted_y-1].isVisited==true && KnownCells[predicted_x][predicted_y-1].isRecentlyVisited==false) //if the left of the predicted AGENTCELL is not at LAST column and already visited and not recently visited
        {
            inferenced_cell = getKnownCell(predicted_x, predicted_y-1); //left of the predicted in this case
            cell_and_mark = consideringProgress(inferenced_cell, predicted);
            if(cell_and_mark.second=='C')
            {
                real_like_prediction = cell_and_mark.first;
                return real_like_prediction;
            }
        }

        if(predicted_x>0 && KnownCells[predicted_x-1][predicted_y].isVisited==true && KnownCells[predicted_x-1][predicted_y].isRecentlyVisited==false) //if the above of the predicted AGENTCELL is not at LAST column and already visited and not recently visited
        {
            inferenced_cell = getKnownCell(predicted_x-1, predicted_y); //above of the predicted in this case
            cell_and_mark = consideringProgress(inferenced_cell, predicted);
            if(cell_and_mark.second=='C')
            {
                real_like_prediction = cell_and_mark.first;
                return real_like_prediction;
            }
        }
        
        if(predicted_x<BORDER-1 && KnownCells[predicted_x+1][predicted_y].isVisited==true && KnownCells[predicted_x+1][predicted_y].isRecentlyVisited==false) //if the below of the predicted AGENTCELL is not at LAST column and already visited and not recently visited
        {
            inferenced_cell = getKnownCell(predicted_x+1, predicted_y); //below of the predicted in this case
            cell_and_mark = consideringProgress(inferenced_cell, predicted);
            if(cell_and_mark.second=='C')
            {
                real_like_prediction = cell_and_mark.first;
                return real_like_prediction;
            }
        }
    return real_like_prediction;
}

void Agent::movement(int x, int y, World world)
{   
    int cord_x = x;
    int cord_y = y;
    std::pair<int, int> coordinates;
    std::vector<std::pair<int, int>> path;
    CELL choosen_cell;
    path.emplace_back(cord_x,cord_y); //Current CELL before moving
    while(getCurrentCell().hasGold < 1)
    {   
        if(getCurrentCell().hasPit == true || getCurrentCell().hasWumpus == true)
        {
            std::cout << "I'm in danger" << std::endl;
            health = health - 1;
            std::cout << health << " health left" << std::endl;
            if(health == 0)
            {
                std::cout << "I died..." << std::endl;
                break;
            }
        }
        predictionProgress(CurrentCell, cord_x, cord_y);
        std::vector<std::pair<AGENTCELL, char>> Cell_and_Direction;

        if(cord_y<BORDER-1)
        {
            Cell_and_Direction.emplace_back(getKnownCell(cord_x, cord_y+1), 'R'); //Right
        }
        if(cord_y>0)
        {
            Cell_and_Direction.emplace_back(getKnownCell(cord_x, cord_y-1), 'L'); //Left
        }
        if(cord_x>0)
        {
            Cell_and_Direction.emplace_back(getKnownCell(cord_x-1, cord_y), 'A'); //Above
        }
        if(cord_x<BORDER-1)
        {
            Cell_and_Direction.emplace_back(getKnownCell(cord_x+1, cord_y), 'B'); //Below
        }

        std::cout << "I am at x: " << cord_x << " y: " << cord_y << std::endl;
        KnownCells[cord_x][cord_y].isRecentlyVisited=true; //Sets current CELL as RecentlyVisited 
        coordinates = decisionMaker(Cell_and_Direction, cord_x, cord_y); //Decides upcoming CELL's coordinates
        cord_x = coordinates.first;
        cord_y = coordinates.second;
        choosen_cell = world.getCell(coordinates.first, coordinates.second); //Gets real values of CELL from World
        AGENTCELL current = CelltoAgentCell(choosen_cell); //Converts choosen_cell to AGENTCELL by it's own values
        setCurrentCell(current); //Current CELL after moving
        path.emplace_back(cord_x,cord_y); 
        if(path.size()>2) //if Agent passed at least 2 Cell
        {
            if(cord_x != path[path.size()-3].first && cord_y != path[path.size()-3].second) //if new
            {
                KnownCells[path[path.size()-3].first][path[path.size()-3].second].isRecentlyVisited = false; //Sets CELL that 2 steps behind the current in PATH as not RecentlyVisited
            }
        }
        std::cout << "I moved to x: " << cord_x << " y: " << cord_y << std::endl;
        updateKnowledge(current, cord_x, cord_y);
        Cell_and_Direction.clear();
    }

    if(getCurrentCell().hasGold==true)
    std::cout << "I found gold at x: " << cord_x << " y: " << cord_y << std::endl;
}

AGENTCELL Agent::CelltoAgentCell(CELL world)
{
    AGENTCELL converted =
            {
                world.hasBreeze,
                world.hasStench,
                world.hasGlitter,
                0,
                0,
                0,
                true,
                true,
                true,
                true
            };
    converted.hasPit = world.hasPit ? 1:-1;
    converted.hasWumpus = world.hasWumpus ? 1:-1;
    converted.hasGold = world.hasGold ? 1:-1;
    
    return converted;
}

void Agent::updateKnowledge(AGENTCELL learned, int x, int y)
{
    KnownCells[x][y] = learned;
}

void Agent::setCurrentCell(AGENTCELL current)
{
    this->CurrentCell=current;
}

AGENTCELL Agent::getCurrentCell()
{
    return this->CurrentCell;
}

AGENTCELL Agent::getKnownCell(int x, int y)
{
    return this->KnownCells[x][y];
}

Agent::Agent(World world)
{
    health = 3;
    stress = 2;
    for (int i = 0; i < BORDER; ++i) {
        for (int j = 0; j < BORDER; ++j) {
            KnownCells[i][j] = 
            {
                false,
                false,
                false,
                0,
                0,
                0,
                false,
                false,
                false,
                false 
            }; // Initialize with empty map
        }
    }
    AGENTCELL current = CelltoAgentCell(world.getCell(0,0));
    // {
    //     start.hasBreeze,
    //     start.hasStench,
    //     start.hasGlitter,
    //     -1,
    //     -1,
    //     -1,
    //     true,
    //     true,
    //     true,
    //     true
    // }; 
    setCurrentCell(current);
    updateKnowledge(current, 0, 0);
    movement(0, 0, world);
}

Agent::~Agent()
{
    //It kills agent methaporicly
}