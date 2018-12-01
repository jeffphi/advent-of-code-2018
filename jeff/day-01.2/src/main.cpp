#include <fstream>
#include <iostream>
#include <string>
#include <map>

using namespace std;
int main() 
{ 
    int curVal = 0;
    map<int, int> tracker;
    bool found = false;

    // Add initial freq of zero 
    tracker.insert(pair<int, int>(0, 0)); 

    while(!found){
        ifstream file("input");
        string str; 
        while (getline(file, str))
        {
            curVal += stoi(str);
            if(tracker.find(curVal) == tracker.end())
            {
                tracker.insert(pair<int, int>(curVal, 0)); 
                //cout << "Cur: " << curVal << "\tNum: " << str << endl;
            }
            else
            {
                cout << "Target Freq: " << curVal << endl;
                return 0;
            }
        }
    }
}
