#include <iostream>
#include <vector>
#include <string>
#include "jefflib.h" 
#include <boost/tokenizer.hpp>
#include <map>
#include <set>

using namespace std;
using namespace boost;

void processNode(vector<int>& data, int& index, int& checksum);

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

    char_separator<char> sep(" ");
    tokenizer< char_separator<char> > tokens(vect[0], sep);
    vector<string> v_temp(tokens.begin(), tokens.end());

    vector<int> data;
    for(auto s : v_temp){
        data.push_back(stoi(s));
    }

    cout << "Data size: " << data.size() << endl;

    int checksum = 0;
    int index = 0;
    processNode(data, index, checksum);
    cout << "Checksum: " << checksum << endl;
}

void processNode(vector<int>& data, int& index, int& checksum){
    int numChildren = data[index];
    int numMeta = data[++index];
    
    for(int i = 0; i < numChildren; i++){
        processNode(data, ++index, checksum);
    }

    for(int i = 0; i < numMeta; i++){
        checksum += data[++index];
    }
}


