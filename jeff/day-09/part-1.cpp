#include <iostream>
#include <vector>
#include <string>
#include "jefflib.h" 
#include <map>
#include <cmath>

using namespace std;

int playGame(int numPlayers, int numMarbles);

int main(){
    // Here are a few more examples:

    // 10 players; last marble is worth 1618 points: high score is 8317
    // 13 players; last marble is worth 7999 points: high score is 146373
    // 17 players; last marble is worth 1104 points: high score is 2764
    // 21 players; last marble is worth 6111 points: high score is 54718
    // 30 players; last marble is worth 5807 points: high score is 37305

    cout << "10 players, 1618 marbles. Expect 8317,   got " << playGame(10, 1618) << endl;
    cout << "13 players, 7999 marbles. Expect 146373, got " << playGame(13, 7999) << endl;
    cout << "17 players, 1104 marbles. Expect 2764,   got " << playGame(17, 1104) << endl;
    cout << "21 players, 6111 marbles. Expect 54718,  got " << playGame(21, 6111) << endl;
    cout << "30 players, 5807 marbles. Expect 37305,  got " << playGame(30, 5807) << endl;
    cout << "My input: 416 players; last marble is worth 71617 points. Hi-score: " << playGame(416, 71617) << endl;
}

int playGame(int numPlayers, int numMarbles)
{
    int currentPlayer = 0;
    int currentMarble = 0;
    vector<int> marbles;
    marbles.push_back(0); // Start off with the first marble placed

    // map<player, score>
    map<int, int> scorecard;

    // Game on!
    for(int marble = 1; marble <= numMarbles; marble++){
        /*
        int i = 0;
        for(auto m : marbles){
            if(i == currentMarble){
                cout << "(" << m << ") ";
            }
            else {
                cout << m << " ";
            }
            i++;
        }
        cout << endl;
        string dummy;
        getline(cin, dummy);
        */

        if(marble % 23 == 0){ 
            // Score marble
            int points = marble;
            
            // Score -7th & remove
            int deletionIndex = (currentMarble -7) % marbles.size();
            deletionIndex = abs(deletionIndex);
            points += marbles[deletionIndex];
            marbles.erase(marbles.begin()+deletionIndex);

            // Update current
            currentMarble = deletionIndex;
            scorecard[currentPlayer] += points;
        }
        else{ // Insert @ +2
            int insertionIndex = (currentMarble + 2) % marbles.size();
            marbles.insert(marbles.begin()+insertionIndex, marble);
            currentMarble = insertionIndex;
        }   

        currentPlayer = (currentPlayer + 1) % numPlayers;
    }

    // Find high score
    int hi_score = 0;
    for(auto item : scorecard){
        if(item.second > hi_score){
            hi_score = item.second;
        }
    }

    return hi_score;
}


