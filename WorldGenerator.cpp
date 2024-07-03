#include <iostream>
#include "World.cpp"
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
class WorldGenerator{
    private:
        static int generateRandomNumber(int range){
           return rand() % range;
        }
        static int generateRandomDirection(int range, int exclusion){
           try{
                std::vector<int> directions {1,2,3,4};
                directions.erase(directions.begin() +  exclusion-1);
                return rand() % directions.size();
            }catch(const std::out_of_range& e){
                std::cerr << e.what() << '\n' << "generateRandomDirection give error" << std::endl;
           }
        }
        static bool inBorder(int x, int y){
            if((x < BORDER && x >= 0) && (y < BORDER && y >= 0)){
                return true;
            }
            return false;
        }
        static bool inExclusions(int check, int container[4]){
            for(int i=0; i<4; i++){
                if(container[i]==check){
                    return true;
                }
            }
            return false;
        }

        static void changeCompass(int arr[], int direction){
            arr[direction] = -1;
        }

        static void resetCompass(int arr[]){
            for (int i = 0; i < 4; ++i){
                arr[i] = 1;
            }
        }

        // returns true if there is nowhere to go
        static bool checkCompass(int arr[]){
            for(int i = 0; i < 4; ++i){
                if(arr[i] == 1){
                    return false;
                }
                return true;
            }
        }        
    public:
        static World newWorld(){
            World temp;
            CELL createdMap[BORDER][BORDER];
            safePath(createdMap);
            //
        }

        static void safePath(CELL map[BORDER][BORDER]){
            std::srand(time(0));
            
            // Setting location of gold.
            std::pair<int, int> locGold;
            locGold.first = generateRandomNumber(BORDER);
            locGold.second = generateRandomNumber(BORDER);
            
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
            std::pair<int, int> locCurrent = std::make_pair(0,0);
            int compass[4] = {1, 1, 1, 1};
            while(true){
                // [1, 1, 1, 1]
                // [1, -1, 1, 1]
                // [-1, -1, -1, -1]
                
                // choose a direction to go
                while(!checkCompass(compass)){
                    int direction = generateRandomNumber(4);
                    if(locCurrent.first == locGold.first && locCurrent.second == locGold.second){
                        break;
                    }
                    
                    switch (direction){

                    case 0: //Go Right
                        if((!inBorder(locCurrent.first, locCurrent.second + 1)) ||
                            visited[locCurrent.first][locCurrent.second + 1] ||
                            visiting[locCurrent.first][locCurrent.second + 1])
                        {
                            changeCompass(compass, direction);
                            break;
                        }
                        locCurrent.first;
                        locCurrent.second++;
                        visiting[locCurrent.first][locCurrent.second] = true;
                        resetCompass(compass);
                        stack.push(locCurrent);
                        break;

                    case 1: //Go Up
                        if((!inBorder(locCurrent.first - 1, locCurrent.second)) ||
                            visited[locCurrent.first - 1][locCurrent.second] ||
                            visiting[locCurrent.first - 1][locCurrent.second])
                        {
                            changeCompass(compass, direction);
                            break;
                        }
                        locCurrent.first--;
                        locCurrent.second;
                        visiting[locCurrent.first][locCurrent.second] = true;
                        resetCompass(compass);
                        stack.push(locCurrent);
                        break;
                        
                    case 2: //Go Left
                        if((!inBorder(locCurrent.first, locCurrent.second - 1)) ||
                            visited[locCurrent.first][locCurrent.second - 1] ||
                            visiting[locCurrent.first][locCurrent.second - 1])
                        {
                            changeCompass(compass, direction);
                            break;
                        }
                        locCurrent.first;
                        locCurrent.second--;
                        visiting[locCurrent.first][locCurrent.second] = true;
                        resetCompass(compass);
                        stack.push(locCurrent);
                        break;

                    case 3: //Go Down
                        if((!inBorder(locCurrent.first + 1, locCurrent.second)) ||
                            visited[locCurrent.first + 1][locCurrent.second] ||
                            visiting[locCurrent.first + 1][locCurrent.second])
                        {
                            changeCompass(compass, direction);
                            break;
                        }
                        locCurrent.first++;
                        locCurrent.second;
                        visiting[locCurrent.first][locCurrent.second] = true;
                        resetCompass(compass);
                        stack.push(locCurrent);
                        break;
                
                    default:
                        break;
                    }
                    
                }

                visited[locCurrent.first][locCurrent.second] = true;
                visiting[locCurrent.first][locCurrent.second] = false;

                stack.pop();
                resetCompass(compass);
                locCurrent = stack.top();
                }
            }
};