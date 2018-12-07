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
    int closestDist = INT_MAX;
    int closestCoord = -1;
    int closestCoordCount = 0;
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
    int minX = INT_MAX, minY = INT_MAX;
    int maxX = 0, maxY = 0;
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

        if(tempCoord.x < minX) minX = tempCoord.x;
        if(tempCoord.x > maxX) maxX = tempCoord.x;
        if(tempCoord.y < minY) minY = tempCoord.y;
        if(tempCoord.y > maxY) maxY = tempCoord.y;
    }
   
    cout << "Min x: " << minX << ", Max x: " << maxX << ", Min y: " << minY << ", Max y: " << maxY << endl;

    // Hack(?) to try and identify which coord are bounded vs. unbounded...
    minX = -2000;
    minY = -2000;
    maxX = 2000;
    maxY = 2000;

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
    // For each location, let's find the closest coord(s)
    for(auto  &location : locs){
        for(int i = 0; i < coords.size(); i++){
            int distance = abs(location.x - coords[i].x) + abs(location.y - coords[i].y);
            if(distance == location.closestDist){
                location.closestCoord = -1;
                location.closestCoordCount++;
            } 
            else if(distance < location.closestDist){
                location.closestDist = distance;
                location.closestCoord = i;
                location.closestCoordCount = 1;
            }
        }
    }
    
    // Finally, let's determine which coord appears most frequently (i.e. has the largest area)
    map<int, int> m; // coordId, count map
    for(auto &location : locs){
        // Need to skip over the edge/infinite coords
        if(location.x == maxX || location.x == minX || location.y == maxY || location.y == minY) continue;

        if(location.closestCoordCount == 1){
            m[location.closestCoord] += 1;
        }
        //cout << "Loc: " << location.x << "," << location.y << ": closestCoord = " << location.closestCoord << ", ClosestCoordCount = " << location.closestCoordCount << endl;
    }

    int maxArea = 0;
    for (const auto &p : m) {
        cout << p.first << ", " << p.second << endl;
        if(p.second > maxArea) maxArea = p.second;
    }
    cout << "Max area: " << maxArea << endl;
}


