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

bool WorldGenerator::inBorder(int x, int y) {
    return (x < BORDER && x >= 0) && (y < BORDER && y >= 0);
}

void WorldGenerator::changeCompass(int arr[], int direction) {
    arr[direction] = -1;
}

void WorldGenerator::resetCompass(int arr[]) {
    for (int i = 0; i < 4; ++i) {
        arr[i] = 1;
    }
}

void WorldGenerator::resetCompass(std::vector<int>& compass) {
    compass = {0, 1, 2, 3};
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
    return !visited[nextCell.first][nextCell.second] && !visiting[nextCell.first][nextCell.second];
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

void WorldGenerator::safePath(World& world) {
    srand(time(0));

    // Setting location of gold.
    std::pair<int, int> locGold;
    locGold.first = generateRandomNumber(BORDER);
    locGold.second = generateRandomNumber(BORDER);
    std::cout << "GOLD => " << locGold.first << locGold.second << std::endl;
    world.setCell(locGold.first, locGold.second, GOLD);

    std::vector<std::vector<bool>> visited(BORDER, std::vector<bool>(BORDER, false));
    std::vector<std::vector<bool>> visiting(BORDER, std::vector<bool>(BORDER, false));

    std::stack<std::pair<int, int>> stack;
    std::pair<int, int> locCurrent = std::make_pair(0, 0);

    int compass[4] = {1, 1, 1, 1};

    while (true) {
        std::cout << "Current cell : (" << locCurrent.first << locCurrent.second << ")" << std::endl;
        if (locCurrent.first == locGold.first && locCurrent.second == locGold.second) {
            std::cout << "Gold found!" << std::endl;
            break;
        }
        setLocationAs(visiting, locCurrent, true);
        stack.push(locCurrent);

        bool isStuck = false;
        int direction = -1;

        do {
            if (checkCompass(compass)) {
                isStuck = true;
                break;
            }
            direction = generateRandomNumber(4);
            if (compass[direction] == -1) continue;
            changeCompass(compass, direction);
        } while (!isWayValid(visited, visiting, nextCell(locCurrent, direction)));

        if (isStuck) {
            stack.pop();
            if (stack.empty()) break; // If stack is empty, no way to proceed
            locCurrent = stack.top();
            setLocationAs(visited, locCurrent, true);
            resetCompass(compass);
            continue;
        }
        setLocationAs(visiting, locCurrent, true);
        resetCompass(compass);
        locCurrent = nextCell(locCurrent, direction);
    }

    // Transfer stack contents to a vector
    std::vector<std::pair<int, int>> path;
    while (!stack.empty()) {
        path.push_back(stack.top());
        stack.pop();
    }

    // Select wumpus and pits
    std::pair<int, int> locWumpus;

    do {
        locWumpus.first = generateRandomNumber(BORDER);
        locWumpus.second = generateRandomNumber(BORDER);
    } while (std::find(path.begin(), path.end(), locWumpus) != path.end() || locWumpus == std::make_pair(0, 0));
    world.setCell(locWumpus.first, locWumpus.second, WUMPUS);

    std::pair<int, int> locPits[PITS];

    for (int i = 0; i < PITS; ++i) {
        do {
            locPits[i].first = generateRandomNumber(BORDER);
            locPits[i].second = generateRandomNumber(BORDER);
        } while (std::find(path.begin(), path.end(), locPits[i]) != path.end() ||
                 locPits[i] == std::make_pair(0, 0) ||
                 includes(locPits, i, locPits[i]) ||
                 locPits[i] == locGold);
        world.setCell(locPits[i].first, locPits[i].second, PIT);
    }

    std::cout << "Wumpus: " << locWumpus.first << "," << locWumpus.second << std::endl;
    for (int i = 0; i < PITS; ++i) {
        std::cout << "Pit" << i + 1 << ": " << locPits[i].first << "," << locPits[i].second << std::endl;
    }
}
