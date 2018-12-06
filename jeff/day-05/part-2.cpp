#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include "jefflib.h" 
#include <algorithm>

using namespace std;
//using namespace boost;

// Ugh, forward declaring a thing...
int getPolymerLength(string input);

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
 
    string input = vect[0];
    int shortest = INT_MAX;
    char targetLetter;
    string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for(int i = 0; i < letters.length(); i++ ){
        string s = input;
        s.erase(remove(s.begin(), s.end(), letters[i]), s.end());
        s.erase(remove(s.begin(), s.end(), tolower(letters[i])),  s.end());
        int len = getPolymerLength(s);
        if (len < shortest){
            shortest = len;
            targetLetter = letters[i];
        }
    }

    cout << "Bad unit: " << targetLetter << ", length: " << shortest << endl;
}

int getPolymerLength(string input)
{
    int index = 0;
    stack<char> stack;
    while(index < input.length()){
        
        if(stack.empty()){
            stack.push(input[index]);
            index++;
            continue;
        }

        char c1 = stack.top();
        char c2 = input[index];
        if(c1 != c2 && tolower(c1, locale()) == tolower(c2, locale())){
            //cout << c1 << " matches " << c2 << endl;
            stack.pop();
        } else {
            stack.push(input[index]);
        }
        index++;
    }

    return stack.size();
}


