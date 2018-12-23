#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "jefflib.h" 

using namespace std;

int printAnswer(string input){
    vector<int> data;
    data.push_back(3);
    data.push_back(7);

    int elfA = 0;
    int elfB = 1;

    while(true){
        int nextRecipe = data[elfA] + data[elfB];
        string s = to_string(nextRecipe);

        if(nextRecipe > 9){
            data.push_back(stoi(s.substr(0,1)));
             if(data.size() > input.length()){
                stringstream s;
                for(int i = data.size()-input.length(); i < data.size(); i++){
                    s << data[i];
                }
                string tail = s.str();
                if(tail.compare(input) == 0){
                    return data.size()-input.length();
                }
            }
        }
        data.push_back(stoi(s.substr(s.length()-1)));
        if(data.size() > input.length()){
            stringstream s;
            for(int i = data.size()-input.length(); i < data.size(); i++){
                s << data[i];
            }
            string tail = s.str();
            if(tail.compare(input) == 0){
                return data.size()-input.length();
            }
        }
        elfA = (elfA + data[elfA] + 1) % data.size();
        elfB = (elfB + data[elfB] + 1) % data.size();
    }
}

int main()
{
    cout << "Expect 9: " << printAnswer("51589") << endl;
    cout << "Expect 5: " << printAnswer("01245") << endl;
    cout << "Expect 18: " << printAnswer("92510") << endl;
    cout << "Expect 2018: " << printAnswer("59414") << endl;
    cout << "Solution: " << printAnswer("880751") << endl;
}
