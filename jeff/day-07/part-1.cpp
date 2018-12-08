#include <iostream>
#include <vector>
#include <string>
#include "jefflib.h" 
#include <boost/tokenizer.hpp>
#include <map>
#include <set>

using namespace std;
using namespace boost;

struct step_t {
    char id;
    set<char> prereqs;
};

void printSteps(map<char, set<char> > &steps){
    for(const pair<char, set<char> > item : steps){
        cout << item.first << ": " << item.second.size() << endl;
    }
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

    map<char, set<char> > steps;

    // .....5..............................36
    // Step V must be finished before step M can begin.
    for (const string line : vect) {
        char c = line[36];
        char p = line[5];
        //cout << c << " deps " << p << endl;
        steps[c].insert(p);

        // So, items without prereqs don't have dependencies, so make sure they get 
        // have entries created as well...
        steps[p];
    }

    cout << "Looking..." << endl;
    while(steps.size() > 0){
        //printSteps(steps);
        // Find ready step
        char ready = ' ';
        for(pair<char, set<char> > item : steps){
            //cout << item.first << ": dep count " << item.second.size() << endl;
            if(item.second.size() == 0){
                ready = item.first;
                cout << ready;
                // Note: this is always the next ready one alphabetically 
                // due to the magic of sorted maps!
                break;
            }
        }

        //cout << "Removing " << ready << " from Steps" << endl;
        // Remove ready step from steps 
        auto it = steps.find(ready);
        steps.erase(it);

        // Remove ready step from deps
        //cout << "Removing " << ready << " from deps" << endl;
        
        for(auto& item : steps){
            auto it = item.second.find(ready);
            if(it != item.second.end()){
               //cout << "Erasing " << ready << ", Num erased: " <<  item.second.erase(*it) << endl;
               item.second.erase(*it);
            }
        }
        

        /*
        for(map<char, set<char> >::iterator mapIt = steps.begin(); mapIt != steps.end(); mapIt++){
            set<char>::iterator setIt = mapIt->second.find(ready);
           // mapIt->second.erase(setIt);
        }
        */
    }

    cout << endl;
}


