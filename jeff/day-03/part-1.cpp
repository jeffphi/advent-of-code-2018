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

    // Count squares with >1 claim
    int count = 0;
    for (int i = 0; i<1000; i++){
        for(int j=0; j<1000;j++){
            if (cloth[i][j] > 1){
                count++;
            }
        }
    }

    cout << "Count: " << count << endl;
}
