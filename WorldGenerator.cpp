#include <iostream>
#include "WorldGenerator.h"
#include "World.h"
#include <stack>
#include <vector>
#include <cstdlib>
#include <time.h>
#define BORDER 5
/*
Rules:
- 1 Wumpus for a map.
- 1 gold for a map.
*/

int WorldGenerator::generateRandomNumber(int range){
    return rand() % range;
}

int WorldGenerator::generateRandomDirection(vector<int>& compass){
    int size = compass.size();
    int random = rand() % size;
    compass.erase(compass.begin() + random);
    return random;
}
bool WorldGenerator::inBorder(int x, int y)
{
    if ((x < BORDER && x >= 0) && (y < BORDER && y >= 0))
    {
        return true;
    }
    return false;
}

void WorldGenerator::changeCompass(int arr[], int direction)
{
    arr[direction] = -1;
}

void WorldGenerator::resetCompass(int arr[])
{
    for (int i = 0; i < 4; ++i)
    {
        arr[i] = 1;
    }
}

void WorldGenerator::resetCompass(vector<int>& compass){
    compass = {0, 1, 2, 3};
}

bool WorldGenerator::checkCompass(vector<int>& compass){
    int size = compass.size();
    if(size == 0) return true;
    return false;
}

// returns true if there is nowhere to go
bool WorldGenerator::checkCompass(int arr[])
{
    for (int i = 0; i < 4; ++i)
    {
        if (arr[i] == 1)
        {
            return false;
        }
        return true;
    }
}

void WorldGenerator::setLocationAs(std::vector<std::vector<bool>>& vec, std::pair<int, int> pair, bool value){
    vec[pair.first][pair.second] = value;
}

bool WorldGenerator::isWayValid(std::vector<std::vector<bool>>& visited, std::vector<std::vector<bool>>& visiting, std::pair<int, int> nextCell){
    if(!inBorder(nextCell.first, nextCell.second)){
        return false;
    }


    return (!visited[nextCell.first][nextCell.second] &&
            !visiting[nextCell.first][nextCell.second]);
}// visited false  and bind true 


std::pair<int, int> WorldGenerator::nextCell(std::pair<int, int> curr, int direction){
    switch (direction){
        case 0: // right
            return std::make_pair(curr.first, curr.second + 1);
        
        case 1: // up
            return std::make_pair(curr.first - 1, curr.second);
        
        case 2: // left
            return std::make_pair(curr.first, curr.second - 1);
        
        case 3: // down
            return std::make_pair(curr.first + 1, curr.second);
    }
}

World WorldGenerator::newWorld()
{
    World temp;
    CELL createdMap[BORDER][BORDER];
    safePath(createdMap);
    //
}

void WorldGenerator::safePath(CELL (*map)[BORDER])
{
    std::srand(time(0));
    
    // Setting location of gold.
    std::pair<int, int> locGold;
    locGold.first = generateRandomNumber(BORDER);
    locGold.second = generateRandomNumber(BORDER);
    std::cout << "GOLD => " << locGold.first << locGold.second << std::endl;

    std::vector<std::vector<bool>> visited(BORDER, std::vector<bool>(BORDER, false));
    std::vector<std::vector<bool>> visiting(BORDER, std::vector<bool>(BORDER, false));
    /*
    false false false false false
    false false false false false
    false false false false false
    false false false false false
    false false false false false
    */
    std::stack<std::pair<int, int>> stack;
    std::pair<int, int> locCurrent = std::make_pair(0, 0);


    //std::vector<int> compass = {0, 1, 2, 3}; //!vector
    int compass[4] = {1, 1, 1, 1};  //! array


    while(true){
        std::cout << "Current cell : (" << locCurrent.first << locCurrent.second << ")" << std::endl;
        if(locCurrent.first == locGold.first && locCurrent.second == locGold.second){
            std::cout << "gold found!" << std::endl;
            break;
        }
        //new loc
        setLocationAs(visiting, locCurrent, true);
        stack.push(locCurrent);
        
        bool isStuck = false;

        int direction = -1;
        int i = 0;
        // choose a direction
        do{
            if(checkCompass(compass)){
                isStuck = true;
                break;
            }
            //!if using with vector uncomment lines below and comment the other pair
            // direction = generateRandomDirection(compass);
            // std::cout << compass[direction]<< std::endl;
            
            //! array
            direction = generateRandomNumber(4);
            changeCompass(compass, direction);


        }while(!isWayValid(visited, visiting, nextCell(locCurrent, direction)));
        
        if(isStuck){
            stack.pop();
            locCurrent = stack.top();
            setLocationAs(visited, locCurrent, true);
            resetCompass(compass);
            continue;
        }

        resetCompass(compass);
        locCurrent = nextCell(locCurrent, direction);
    }
}
