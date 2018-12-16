#include <iostream>
#include <vector>
#include <string>
#include "jefflib.h" 
#include <boost/tokenizer.hpp>

using namespace std;

int getHundred(int x){
    string s = to_string(x);
    if(s.size()>= 3){
        return stoi(s.substr(s.size() - 3, 1));
    }
    else{
        return 0;
    }
}

int main()
{
    int rack = 0;
    int pwr = 0;
    int maxPwr = 0;
    int maxX = 0;
    int maxY = 0;
    int serialNum = 2568;
    int grid[301][301];

    for(int x = 1; x <= 300; x++){
        for(int y = 1; y <= 300; y++){
            rack = x + 10;
            pwr = rack * y;
            pwr += serialNum;
            pwr *= rack;
            pwr = getHundred(pwr) - 5;
            grid[x][y] = pwr;
        }
    }

    for(int x = 1; x <= 298; x++){
        for(int y = 1; y <= 298; y++){
            pwr = grid[x][y] + grid[x+1][y] + grid[x+2][y];
            pwr = pwr + grid[x][y+1] + grid[x+1][y+1] + grid[x+2][y+1];
            pwr = pwr + grid[x][y+2] + grid[x+1][y+2] + grid[x+2][y+2];
            if(pwr > maxPwr){
                maxPwr = pwr;
                maxX = x;
                maxY = y;
            }
        }
    }
    cout << "Part 1: Max power cell = " << maxX << ", " << maxY << endl; 

    // Part 2
    maxPwr = 0;
    int maxSz = 0;
    for(int x = 1; x <= 300; x++){
        cout << x << ": ";
        for(int y = 1; y <= 300; y++){
            cout << y;
            // For each cell, calc power for all the valid square sizes
            int maxDim = 0;
            int maxXDim = 300 - x + 1;
            int maxYDim = 300 - y + 1;
            if(maxXDim > maxYDim){
                maxDim = maxYDim;
            } 
            else{
                maxDim = maxXDim;
            }

            for(int sz = 1; sz <= maxDim; sz++){
                pwr = 0;
                for(int row = 0; row < sz; row++){
                    for(int col = 0; col < sz; col++){
                        pwr = pwr + grid[x+col][y+row];
                    }
                }
                if(pwr > maxPwr){
                    maxPwr = pwr;
                    maxSz = sz;
                    maxX = x;
                    maxY = y;
                }
            }
        }
        cout << endl;
    }
    cout << "Part 2: Max power cell = " << maxX << "," << maxY << "," << maxSz << endl;
}


