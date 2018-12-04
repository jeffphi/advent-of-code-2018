#include <iostream>
#include <vector>
#include "jefflib.h" 
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;

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

    // Create a 1000x1000 piece of cloth
    int cloth[1000][1000] = {0};

    // Accumulate claims
    for (const string line : vect)
    {
        char_separator<char> sep(" @,:x");
        tokenizer< char_separator<char> > tokens(line, sep);
        // num col row width height
        vector<string> s_tmp(tokens.begin(), tokens.end());
        int col_offset = stoi(s_tmp[1]);
        int row_offset = stoi(s_tmp[2]);
        int width = stoi(s_tmp[3]);
        int height = stoi(s_tmp[4]);

        for(int col = col_offset; col < col_offset+width; col++){
            for(int row = row_offset; row<row_offset+height; row++){
                cloth[row][col] += 1;
            }
        }
    }

    // Find claim
    for (const string line : vect)
    {
        char_separator<char> sep(" @,:x");
        tokenizer< char_separator<char> > tokens(line, sep);
        // num col row width height
        vector<string> s_tmp(tokens.begin(), tokens.end());
        string id = s_tmp[0];
        int col_offset = stoi(s_tmp[1]);
        int row_offset = stoi(s_tmp[2]);
        int width = stoi(s_tmp[3]);
        int height = stoi(s_tmp[4]);

        bool perfect = true;
        for(int col = col_offset; col < col_offset+width; col++){
            for(int row = row_offset; row<row_offset+height; row++){
                if(cloth[row][col] != 1){
                    perfect = false;
                }
            }
        }

        if(perfect){
            cout << "Found it! " << id << endl;
            return 0;
        }
    }


    cout << "Count: " << cout << endl;
}
