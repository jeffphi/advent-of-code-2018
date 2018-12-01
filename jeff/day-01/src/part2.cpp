#include <fstream>
#include <iostream>
#include <string>

using namespace std;
int main() 
{ 
    ifstream file("input");
    string str; 
    int curVal = 0;
    map<int, int> tracker;
    bool found = false;

    // Add initial freq of zero 
    tracker.insert(pair<int, int>(0, 0)); 

    while(!found){
        while (getline(file, str))
        {
            curVal += stoi(str);
            if(tracker.find(curVal) == tracker.end())
            {
                tracker.insert(pair<int, int>(curVal, 0)); 
            }
            else
            {
                cout << "Freq: " << curVal << endl;
                found = true;
            }
        }
    }
}
