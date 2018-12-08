#include <iostream>
#include <vector>
#include <string>
#include "jefflib.h" 
#include <boost/tokenizer.hpp>
#include <map>
#include <set>
#include <iomanip>
#include <thread>
#include <chrono>

using namespace std;
using namespace boost;

struct stepInfo_t {
    set<char> prereqs;
    bool workerAssigned = false;
    int clock = 0;
};

void ClearScreen()
{
    cout << string( 100, '\n' );
}

void printSteps(map<char, stepInfo_t > &steps){
    ClearScreen();
    for(const auto item : steps){
        //cout << item.first << ": " << item.second.size() << endl;
        cout << item.first;
        
        if(item.second.workerAssigned){
            cout << " w ";
        }
        else {
            cout << " . ";
        }

        cout << setfill('0') << setw(2) << item.second.clock;
        cout << " [";    
        for(auto prereq : item.second.prereqs){
            cout << prereq;
        }

        cout << "]" << endl;
    }
    cout << endl;
}

int main()
{
    vector<string> vect;
    if(GetStringInput(vect)){
        cout << "Got data!" << endl;
        cout << endl;
    }
    else {
        cout << "Failed to read input :( " << endl;
        return -1;
    }

    map<char, stepInfo_t > steps;

    // .....5..............................36
    // Step V must be finished before step M can begin.
    for (const string line : vect) {
        char c = line[36];
        char p = line[5];
        //cout << c << " deps " << p << endl;
        steps[c].prereqs.insert(p);

        // So, items without prereqs don't have dependencies, so make sure they get 
        // have entries created as well...
        steps[p];
    }
    
    string dummy;
    getline(cin, dummy);

    //cout << "Working..." << endl;
    int elapsedTime = 0;
    int workers = 5; 
    while(!steps.empty()){
        printSteps(steps);
        cout << "Elapsed time: " << elapsedTime << endl;

        this_thread::sleep_for (std::chrono::milliseconds(100));

        // Find ready step
        char ready = ' ';

        // Assing workers to any ready steps
        //cout << "Assigning workers..." << endl;
        for(auto& item : steps){
            if(item.second.prereqs.size() == 0 && !item.second.workerAssigned && workers > 0){
                ready = item.first;
                item.second.workerAssigned = true;
                workers--;
                // ASCII A = 65, so always subtract 4
                item.second. clock = int(item.first) - 4;
            }
        }

        // Decrement clocks
        //cout << "Decrementing clocks..." << endl;
        for(auto& item : steps){
            if(item.second.clock >  0){ 
                item.second. clock -= 1; 
            }
        }

        // If any steps are done, remove them and free up workers!
        //cout << "Checking for completed steps, size = " << steps.size() << endl;
        for(auto& item : steps){
            if(item.second.workerAssigned && item.second.clock == 0){            
                workers++;
                // Remove step from other step's dependendencies
                char ready = item.first;
                printSteps(steps);
                //cout << "Removing " << ready << " from deps" << endl;
                for(auto& item : steps){
                    auto it = item.second.prereqs.find(ready);
                    if(it != item.second.prereqs.end()){
                        //cout << "Erasing " << ready << ", Num erased: " <<  item.second.erase(*it) << endl;
                        item.second.prereqs.erase(*it);
                    }
                }

                //Remove step from list of steps
                //cout << "Removing " << ready << " from steps" << endl;
                auto it = steps.find(ready);
                if(it != steps.end()){
                    //cout << "Erasing " << ready;
                    steps.erase(it);
                    //cout << "... Erased!" << endl;

                    /*
                    // HACKING
                    if(ready == 'M'){
                        cout << "Removed M. A clock: " << steps['A'].clock << endl;
                        cout << "Elapsed time: " << elapsedTime << endl;
                    }
                    */
                } 
                else {
                    cout << "Hmm, couldn't remove " << ready << "??" << endl;   
                }
            }
            //cout << "Continuing loop..." << endl;
        }

        // Increment overall elapsed time
        //cout << "Incrementing time from " << elapsedTime << endl;
        elapsedTime++;
    }

    cout << "Elapsed time: " << elapsedTime << endl;
}

