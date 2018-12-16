#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "jefflib.h" 
#include <map>

using namespace std;

int main()
{
    vector<string> input;
    if(GetStringInput(input)){
        cout << "Got data!" << endl;
        cout << endl;
    }
    else {
        cout << "Failed to read input :( " << endl;
        return -1;
    }

    // First line looks like
    //...............15.......... 
    //initial state: <data>
    string initState = input[0].substr(15);
    cout << initState << endl;

    // Third line and beyond looks like
    //..### => .
    map<string, string> rules;
    for(int i = 2; i < input.size(); i++){
        rules[input[i].substr(0,5)] = input[i].substr(9,1);
    }

    int numGens = 300;
    int bufferSize = numGens+2 + initState.size() + numGens+2;
    cout << "BuffSz: " << bufferSize << " InitInput: " << initState.size() <<  endl;
    string curState[bufferSize];
    string nextState[bufferSize];
    string *pCur  = curState;
    string *pNext = nextState;

    // Init buffers
    for(int i = 0; i<bufferSize; i++){
        pCur[i] = ".";
        pNext[i] = ".";
    }

    // Copy initial input to pCur
    cout << "pCur: ";
    for(int i = 0; i < initState.size(); i++){
        pCur[i+numGens+2] = initState[i];
        cout << pCur[i+numGens+2];
    }
    cout << endl;
    
    for(int i = 0; i < 300/*numGens*/; i++){
        for(int index = 2; index < bufferSize -2; index++){
            ostringstream os;
            for(int j = index - 2; j <= index + 2; j++){
                os << pCur[j];
            }
            string pattern = os.str();
            //cout << pattern << ":";
            pNext[index] = rules[pattern];
            //cout  << rules[pattern] << endl;
        }
        /*
         for(int i = 0; i<bufferSize; i++){
            cout<<pCur[i];
         }
         cout << endl;
        */

        for(int i = 0; i<bufferSize; i++){
            cout<<pNext[i];
        }
        cout << endl << endl;
   
        cout << "Generation " << i+1 << endl;
        
        //cout << "Swapping..." << endl;
        // Swap buffers...
        string *pTemp = pCur;
        pCur = pNext;
        pNext = pTemp;

        //string in;
        //getline(cin,in);
        // Count plants in current buffer
        int count = 0;
        int id = 0 - (numGens+2);
        for(int i = 0; i < bufferSize; i++){
            cout << pCur[i];
            if(pCur[i] == "#") count += id;
                id++;
        }

        cout << endl;

        cout << "Number of Plants: " << count << endl;

    }


}
