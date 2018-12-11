#include <iostream>
#include <list>
#include <string>
#include "jefflib.h" 
#include <map>
#include <cmath>

using namespace std;

long playGame(int numPlayers, int numMarbles);

struct node_t{
    long value = 0;
    node_t* pNext;
    node_t* pPrev;
};

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
    cout << "My input: 416 players; last marble is worth 7161700 points. Hi-score: " << playGame(416, 7161700) << endl;
}

long playGame(int numPlayers, int numMarbles)
{
    int currentPlayer = 0;

    // Start off with the first marble placed
    node_t* currentNode = new node_t;
    currentNode->value = 0;
    currentNode->pNext = currentNode;
    currentNode->pPrev = currentNode;

    // map<player, score>
    map<int, long> scorecard;

    // Game on!
    for(int marble = 1; marble <= numMarbles; marble++){
        if(marble % 23 == 0){ 
            // Score marble
            int points = marble;
            
            // Score -7th & remove
            for(int i = 0; i < 7; i++){
                currentNode = currentNode->pPrev;
            }
            points += currentNode->value;
            
            currentNode = currentNode->pNext;
            currentNode->pPrev = currentNode->pPrev->pPrev;
            currentNode->pPrev->pNext = currentNode;
            // TODO: don't leak old nodes?

            scorecard[currentPlayer] += points;
        }
        else{ // Insert @ +2
            node_t* newNode = new node_t;
            newNode->value = marble;
            currentNode = currentNode->pNext;
            newNode->pPrev = currentNode;
            newNode->pNext = currentNode->pNext;
            currentNode->pNext->pPrev = newNode;
            currentNode->pNext = newNode;
            currentNode = newNode;
        }   

        currentPlayer = (currentPlayer + 1) % numPlayers;
    }

    // Find high score
    long hi_score = 0;
    for(auto item : scorecard){
        if(item.second > hi_score){
            hi_score = item.second;
        }
    }

    return hi_score;
}


