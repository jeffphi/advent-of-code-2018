#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "jefflib.h" 

using namespace std;

string printAnswer(int input){
    vector<int> data;
    data.push_back(3);
    data.push_back(7);

    int elfA = 0;
    int elfB = 1;
    const int answerLen = 10;

    while(data.size() <= (input + answerLen)){
        /*cout << "A "<< elfA <<","<<data[elfA] << endl;
        cout << "B "<< elfB <<","<<data[elfB] << endl;
        for(auto x : data){
            cout << x << " ";
        }
        cout << endl;
        string foo;
        getline(cin, foo);
        */
        int nextRecipe = data[elfA] + data[elfB];
        string s = to_string(nextRecipe);

        if(nextRecipe > 9){
            data.push_back(stoi(s.substr(0,1)));
        }
        data.push_back(stoi(s.substr(s.length()-1)));
        elfA = (elfA + data[elfA] + 1) % data.size();
        elfB = (elfB + data[elfB] + 1) % data.size();
    }

    stringstream s;
    for(int i = input; i < input+answerLen; i++){
        s << data[i];
    }
    return s.str();
}

int main()
{
    cout << "Expect 5158916779: " << printAnswer(9) << endl;
    cout << "Expect 0124515891: " << printAnswer(5) << endl;
    cout << "Expect 9251071085: " << printAnswer(18) << endl;
    cout << "Expect 5941429882: " << printAnswer(2018) << endl;
    cout << "Solution: " << printAnswer(880751) << endl;
}
