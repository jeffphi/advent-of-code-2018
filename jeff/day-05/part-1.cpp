#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include "jefflib.h" 

using namespace std;
//using namespace boost;

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
            cout << c1 << " matches " << c2 << endl;
            stack.pop();
        } else {
            stack.push(input[index]);
        }
        index++;
    }
    cout << "Done: " << stack.size() <<  endl; 
}


