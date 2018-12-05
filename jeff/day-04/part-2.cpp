#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "jefflib.h" 

using namespace std;
//using namespace boost;

// Parsing a line like 
//      [1518-03-15 00:01] Guard #709 begins shift 
// to get the guard's #
int getGuardId(string& line){
    size_t start = line.find("#");
    size_t end = line.find("begins");
    return stoi(line.substr(start+1, end-start));
}

// Parsing a line like 
//      [1518-03-15 00:12] falls asleep 
// to get the seconds value
int getTime(string line){
    size_t start = line.find(":");
    return stoi(line.substr(start+1, 2));
}

void processEntry(int guardId, int index, vector<string>& data, map<int, int>& sleepCounter, map<int, map<int,int> >& minuteCounter){
    int sleep = getTime(data[index]);
    int wake = getTime(data[index+1]);

    sleepCounter[guardId] += wake-sleep;
    for(int i=sleep; i<wake; i++){
        minuteCounter[guardId][i] += 1;
    }
    return;
}

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
    
    int index = 0;
    map<int, int> sleepCounter;
    map<int, map<int, int> > minuteCounter;
    while(index < vect.size()){
        // Inital entry is a Guard beginning a shift...
        int guardId = getGuardId(vect[index]);
        //cout << "Id: " << guardId << endl;
        index++;
        while(vect[index].find("falls") != std::string::npos){
            processEntry(guardId, index, vect, sleepCounter, minuteCounter);
            index+=2;
        }
    }

    // Find guard who has the most minutes of sleep
    int maxSeconds = 0;
    int maxGuard = 0;
    for (pair<int, int> item : sleepCounter) {
        if(item.second > maxSeconds){
            maxSeconds = item.second;
            maxGuard = item.first;
        }
	}

    // Find key minute
    int maxCount = 0;
    int keyMinute = 0;
    for(pair<int, int> item : minuteCounter[maxGuard]){
        if(item.second > maxCount){
            maxCount = item.second;
            keyMinute = item.first;
        }
    }
    cout << "Done! " << "Guard: "<< maxGuard << ", Seconds: " << maxSeconds << ", Minute: " << keyMinute << endl;

    //Part 2: Max Minute Overall
    int keyGuard = 0;
    keyMinute = 0;
    maxCount = 0;
    for(pair<int, map<int, int> > guardData : minuteCounter){
        for(pair<int, int> item : guardData.second){
            if(item.second > maxCount){
                maxCount = item.second;
                keyMinute = item.first;
                keyGuard = guardData.first;
            }
        }
    }

    cout << "Part 2 Guard: " << keyGuard << ", Minute: " << keyMinute << ", Answer: " << keyGuard*keyMinute << endl; 
}


