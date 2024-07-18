#ifndef WORLDGENERATOR_H
#define WORLDGENERATOR_H

#include "World.h"
#include <utility>
#include <vector>

#define PITS 3
#define BORDER 5

class WorldGenerator {
public:
    static World newWorld();
private:
    static int generateRandomNumber(int range);
    static int generateRandomDirection(std::vector<int>& compass);
    static bool inBorder(int x, int y);
    static void changeCompass(int arr[], int direction);
    static void resetCompass(int arr[]);
    static void resetCompass(std::vector<int>& compass);
    static bool checkCompass(int arr[]);
    static bool checkCompass(std::vector<int>& compass);
    static void setLocationAs(std::vector<std::vector<bool>>& vec, std::pair<int, int> pair, bool value);
    static bool isWayValid(std::vector<std::vector<bool>>& visited, std::vector<std::vector<bool>>& visiting, std::pair<int, int> nextCell);
    static std::pair<int, int> nextCell(std::pair<int, int> curr, int direction);
    static bool includes(std::pair<int, int> array[], int size, std::pair<int, int> value);
    static void safePath(World& world);
    static bool checkCross(const World& World, const std::pair<int, int>& coord);
};

#endif
