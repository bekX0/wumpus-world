#ifndef WORLDGENERATOR_H
#define WORLDGENERATOR_H
#define BORDER 5
#include "World.h"
#include <vector>

using namespace std;
class WorldGenerator{
    private:
        static int generateRandomNumber(int range);
        static int generateRandomDirection(vector<int>& compass);
        static bool inBorder(int x, int y);
        static bool inExclusions(int check, int container[4]);
        static void changeCompass(int arr[], int direction);
        static void resetCompass(int arr[]);
        static void resetCompass(std::vector<int>&);
        static bool checkCompass(int arr[]);
        static bool checkCompass(std::vector<int>&);
        static void setLocationAs(std::vector<std::vector<bool>>&, std::pair<int, int>, bool);
        static bool isWayValid(std::vector<std::vector<bool>>&, std::vector<std::vector<bool>>&, std::pair<int, int>);
        static std::pair<int, int> nextCell(std::pair<int, int> curr, int direction);
    public:
        static World newWorld();
        static void safePath(CELL map[BORDER][BORDER]); 
        
};
#endif