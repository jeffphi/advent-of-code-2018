#include <iostream>
#include <vector>
#include <string>
#include "jefflib.h" 
#include <boost/tokenizer.hpp>
#include <cmath>
#include <map>

using namespace std;
using namespace boost;

struct coord_t {
    int x;
    int y;
};

struct loc_t {
    int x;
    int y;
    int distSum = 0;
};

int main()
{
    vector<string> vect;
    if(GetStringInput(vect)){
        cout << "Got data!" << endl;
        cout << endl;
    }
    else {
        cout << "Failed to read input :( " << cout;
        return -1;
    }
   
    // Create the list of coords, making note of bounds
    vector<coord_t> coords;
    for (const string line : vect)
    {
        char_separator<char> sep(",");
        tokenizer< char_separator<char> > tokens(line, sep);
        // x y
        vector<string> s_tmp(tokens.begin(), tokens.end());
        coord_t tempCoord;
        tempCoord.x = stoi(s_tmp[0]);
        tempCoord.y = stoi(s_tmp[1]);
        coords.push_back(tempCoord);
    }
   

    int minX = -100;
    int minY = -100;
    int maxX = 1000;
    int maxY = 1000;

    // Let's build a (bounded) list of locations of interest
    vector<loc_t> locs;
    for(int x = minX; x <= maxX; x++){
        for(int y = minY; y <= maxY; y++){
            loc_t tempLoc;
            tempLoc.x = x;
            tempLoc.y = y;
            locs.push_back(tempLoc);
        }
    }
    cout << "Number of locations: " << locs.size() << endl;
    int regionSize = 0;
    // For each location, let's calculate the sum of distance to each coordinate
    for(auto  &location : locs){
        int distanceSum = 0;
        for(int i = 0; i < coords.size(); i++){
            distanceSum += abs(location.x - coords[i].x) + abs(location.y - coords[i].y);
            if(distanceSum > 10000) break;
        }
        if(distanceSum < 10000) regionSize++;
    }
    
    cout << "Region size: " << regionSize << endl;
}


