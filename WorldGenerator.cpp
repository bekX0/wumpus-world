#include "WorldGenerator.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <stack>
#include <utility>
#include <algorithm>

int WorldGenerator::generateRandomNumber(int range) {
    return rand() % range;
}

int WorldGenerator::generateRandomDirection(std::vector<int>& compass) {
    int size = compass.size();
    int randomIndex = rand() % size;
    int direction = compass[randomIndex];
    compass.erase(compass.begin() + randomIndex);
    return direction;
}
std::pair<int, int> WorldGenerator::generateRandomCell(std::vector<std::pair<int, int>>& available) {
    int size = available.size();
    int randomIndex = rand() % size;
    std::pair<int, int> cell = available[randomIndex];
    available.erase(available.begin() + randomIndex);
    return cell;
}

bool WorldGenerator::inBorder(int x, int y) {
    return (x < BORDER && x >= 0) && (y < BORDER && y >= 0);
}

void WorldGenerator::changeCompass(int arr[], int direction) {
    arr[direction] = -1;
}

void WorldGenerator::changeCompass(std::vector<int>& compass, int direction) {
    int size = compass.size();

    for(int i = 0; i < size; ++i){
        if(direction == compass[i]){
            compass.erase(compass.begin() + direction);
        }
    }
}

void WorldGenerator::resetCompass(int arr[]) {
    for (int i = 0; i < 4; ++i) {
        arr[i] = 1;
    }
}

void WorldGenerator::resetCompass(std::vector<int>& compass) {
    compass.clear();            // Vektörü temizle
    compass = {0, 1, 2, 3};    // Yeni değerlerle doldur
}

bool WorldGenerator::checkCompass(int arr[]) {
    for (int i = 0; i < 4; ++i) {
        if (arr[i] == 1) {
            return false;
        }
    }
    return true;
}

bool WorldGenerator::checkCompass(std::vector<int>& compass) {
    return compass.empty();
}

void WorldGenerator::setLocationAs(std::vector<std::vector<bool>>& vec, std::pair<int, int> pair, bool value) {
    vec[pair.first][pair.second] = value;
}

bool WorldGenerator::isWayValid(std::vector<std::vector<bool>>& visited, std::vector<std::vector<bool>>& visiting, std::pair<int, int> nextCell) {
    if (!inBorder(nextCell.first, nextCell.second)) {
        return false;
    }
    if(visited[nextCell.first][nextCell.second] || visiting[nextCell.first][nextCell.second]){
        return false;
    }
    return true;
}

std::pair<int, int> WorldGenerator::nextCell(std::pair<int, int> curr, int direction) {
    switch (direction) {
        case 0: // right
            return std::make_pair(curr.first, curr.second + 1);
        case 1: // up
            return std::make_pair(curr.first - 1, curr.second);
        case 2: // left
            return std::make_pair(curr.first, curr.second - 1);
        case 3: // down
            return std::make_pair(curr.first + 1, curr.second);
        default:
            return curr; // Invalid direction
    }
}

//Returns true if cross coordinates of cell<coord> are clear and there is no pit or wumpus. TRUE MEANS ITS SAFE
bool WorldGenerator::checkCross(const World& World, const std::pair<int, int>& coord){
    //amaç çaprazda main mob var mı (PIT; WUMPUS)
    /*
    [][][]
    [][][]
    [][][]
    
    upper left => UL 
    upper right => UR
    lower left >= LL
    lower rigt => LR
    */ 
    int x = coord.first;
    int y = coord.second;

    std::pair<int, int> UL = std::make_pair(x-1, y-1);
    std::pair<int, int> UR = std::make_pair(x-1, y+1);
    std::pair<int, int> LL = std::make_pair(x+1, y-1);
    std::pair<int, int> LR = std::make_pair(x+1, y+1);

    std::pair<int, int>* coords = new std::pair<int, int>[4];
    coords[0] = UL;
    coords[1] = UR;
    coords[2] = LL;
    coords[3] = LR;

    for(int i=0; i < 4; ++i){
        
        x = coords[i].first;
        y = coords[i].second;

        if(!inBorder(x, y) || // if cross cell is not inside of the map
            World.getCell(x, y).hasWumpus || // if there is a wumpus
            World.getCell(x, y).hasPit
            )
            {
                return false;
            }
    }
    return true;
}

bool WorldGenerator::includes(std::pair<int, int> array[], int size, std::pair<int, int> value) {
    for (int i = 0; i < size; ++i) {
        if (array[i] == value) {
            return true;
        }
    }
    return false;
}

World WorldGenerator::newWorld() {
    World world;
    safePath(world);
    return world;
}

void WorldGenerator::displayWorld(std::vector<std::pair<int, int>> path){
    std::vector<std::vector<int>> map(BORDER, std::vector<int>(BORDER, 0));
    int size = path.size();
    for(int i = size-1 ; i>=0; --i){
         map[path[i].first][path[i].second] = size--;
    }

    for (int i = 0; i < BORDER; ++i) {
        for (int j = 0; j < BORDER; ++j) {
            std::cout << "[" << map[i][j] << "] ";
        }
        std::cout << std::endl;
    }
}

void WorldGenerator::safePath(World& world) {
    srand(time(0));

    // Setting location of gold.
    std::pair<int, int> locGold;
    locGold.first = generateRandomNumber(BORDER);
    locGold.second = generateRandomNumber(BORDER);
    std::cout << "GOLD => " << locGold.first << locGold.second << std::endl;
    CELL goldCell = {false, false, false, false, false, true, false, true};
    world.setCell(locGold.first, locGold.second, goldCell);
    //CELL glitterCell = {false, false, false, false, true, false, false, true};
    world.setCell(locGold.first+1, locGold.second, 'G');
    world.setCell(locGold.first-1, locGold.second, 'G');
    world.setCell(locGold.first, locGold.second+1, 'G');
    world.setCell(locGold.first, locGold.second-1, 'G');

    std::vector<std::vector<bool>> visited(BORDER, std::vector<bool>(BORDER, false));
    std::vector<std::vector<bool>> visiting(BORDER, std::vector<bool>(BORDER, false));

    std::stack<std::pair<int, int>> stack;
    std::pair<int, int> locCurrent = std::make_pair(0, 0);
    stack.push(locCurrent);
    // int compass[4] = {1, 1, 1, 1};
    std::vector<int> compass = {1, 2, 3, 4};
    int ctr = 0;
    bool isStuck = false;
    while (true) {
        std::cout << "Current cell : (" << locCurrent.first << locCurrent.second << ")" << "compğass:" << compass[0] << compass[1] << compass[2] << compass[3] << "denemeler >=> "<< ctr<< std::endl;
        if (locCurrent.first == locGold.first && locCurrent.second == locGold.second) {
            std::cout << "Gold found!" << std::endl;
            break;
        }
        setLocationAs(visiting, locCurrent, true);
        
        
        int direction = -1;

        do {
            if (checkCompass(compass)) {
                isStuck = true;
                std::cout << "SA AGA" << std::endl;
                break;
            }
            direction = generateRandomDirection(compass);
            changeCompass(compass, direction);
            ctr++;
        } while (!isWayValid(visited, visiting, nextCell(locCurrent, direction)));

        if (isStuck) {
            setLocationAs(visited, locCurrent, true);
            stack.pop();
            if (stack.empty()) break; // If stack is empty, no way to proceed
            locCurrent = stack.top();
            resetCompass(compass);
            isStuck = false;
            continue;
        }
        setLocationAs(visiting, locCurrent, true);
        isStuck = false;
        resetCompass(compass);
        locCurrent = nextCell(locCurrent, direction);
        stack.push(locCurrent);
    }

    // Transfer stack contents to a vector
    std::vector<std::pair<int, int>> path;
    while (!stack.empty()) {
        path.push_back(stack.top());
        stack.pop();
    }

    displayWorld(path);
    // Select wumpus and pits
    std::pair<int, int> locWumpus;

    std::vector<std::pair<int, int>> available; 
    for(int i=0; i< BORDER; ++i){
        for(int j = 0; j <BORDER; ++j){
            std::pair<int, int> cell = std::make_pair(i, j);
            if(std::find(path.begin(), path.end(), cell) == path.end())
                available.push_back(cell);
        }
    }
    if(available.size() < 4){
        safePath(world);
        return;
    }

    // do {
    //     locWumpus.first = generateRandomNumber(BORDER);
    //     locWumpus.second = generateRandomNumber(BORDER);
    // } while (std::find(path.begin(), path.end(), locWumpus) != path.end() || locWumpus == std::make_pair(0, 0));
    locWumpus = generateRandomCell(available);
    CELL wumpusCell = {false, false, false, true, false, false, false, true};
    world.setCell(locWumpus.first, locWumpus.second, wumpusCell);
    //CELL stenchCell = {false, true, false, false, false, false, false, true};
    world.setCell(locWumpus.first+1, locWumpus.second, 'S');
    world.setCell(locWumpus.first-1, locWumpus.second, 'S');
    world.setCell(locWumpus.first, locWumpus.second+1, 'S');
    world.setCell(locWumpus.first, locWumpus.second-1, 'S');

    std::pair<int, int> locPits[PITS];

    for (int i = 0; i < PITS; ++i) {
        // do {
        //     locPits[i].first = generateRandomNumber(BORDER);
        //     locPits[i].second = generateRandomNumber(BORDER);
        // } while (std::find(path.begin(), path.end(), locPits[i]) != path.end() ||
        //          locPits[i] == std::make_pair(0, 0) ||
        //          includes(locPits, i, locPits[i]) ||
        //          locPits[i] == locGold ||
        //          locPits[i] == locWumpus ||
        //          !checkCross(world, locPits[i]));
        locPits[i] = generateRandomCell(available);
        CELL pitCell = {false, false, true, false, false, false, false, true};

        world.setCell(locPits[i].first, locPits[i].second, pitCell);

        //CELL breezeCell = {true, false, false, false, false, false, false, true};
        world.setCell(locPits[i].first+1, locPits[i].second, 'B');
        world.setCell(locPits[i].first-1, locPits[i].second, 'B');
        world.setCell(locPits[i].first, locPits[i].second+1, 'B');
        world.setCell(locPits[i].first, locPits[i].second-1, 'B');

    }

    std::cout << "Wumpus: " << locWumpus.first << "," << locWumpus.second << std::endl;
    for (int i = 0; i < PITS; ++i) {
        std::cout << "Pit" << i + 1 << ": " << locPits[i].first << "," << locPits[i].second << std::endl;
    }
}
