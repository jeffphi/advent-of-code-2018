#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "jefflib.h" 
#include <map>

using namespace std;

// Carts: < > ^ v
// Tracks: - | + \ /

// Intersection sequence: Left, Straight, Right
struct cart{
    char dir = '?';
    char nextInt = 'L';
    bool hasMoved = false;
    bool alive = true;
    int row;
    int col;
};

struct track{
    char type = '?';
    cart* pCart = NULL;
};

void printMine(vector< vector<track> >& grid){
    cout << "Printing mine..." << endl;
     for(int r = 0; r < grid.size(); r++){
        for(int c = 0; c < grid[0].size(); c++){
            if(grid[r][c].pCart != NULL){
                cout << grid[r][c].pCart->dir;
            }
            else{
                cout << grid[r][c].type;
            }
        }
        cout << endl;
    }
}

void printCarts(vector<cart*>& carts){
    cout << "Carts: ";
    for(auto cart : carts){
        if(cart->alive){
            cout << cart->col << "," << cart->row << " ";
        }
    }
    cout << endl;
}

bool moveUp(vector< vector<track> >& grid, int row, int col){
    //cout << "Cart at " << col << "," << row << " moving up..." << endl;
    if(grid[row-1][col].pCart != NULL){
        cout << "Collision at " << col << "," << row-1 << endl;
        grid[row][col].pCart->alive = false;
        grid[row-1][col].pCart->alive = false;
        grid[row][col].pCart = NULL;
        grid[row-1][col].pCart = NULL;
        return true;
    } else{
        track &t = grid[row][col];
        grid[row-1][col].pCart = t.pCart;
        t.pCart->hasMoved = true;
        t.pCart->row = row-1;
        t.pCart->col = col;
        t.pCart = NULL;
        return true;
    }
}

bool moveDown(vector< vector<track> >& grid, int row, int col){
    //cout << "Cart at " << col << "," << row << " moving down..." << endl;
    if(grid[row+1][col].pCart != NULL){
        cout << "Collision at " << col << "," << row+1 << endl;
        grid[row][col].pCart->alive = false;
        grid[row+1][col].pCart->alive = false;
        grid[row][col].pCart = NULL;
        grid[row+1][col].pCart = NULL;
        return true;
    } else{
        track &t = grid[row][col];
        grid[row+1][col].pCart = t.pCart;
        t.pCart->hasMoved = true;
        t.pCart->row = row+1;
        t.pCart->col = col;
        t.pCart = NULL;
        return true;
    }
}

bool moveLeft(vector< vector<track> >& grid, int row, int col){
    //cout << "Cart at " << col << "," << row << " moving left..." << endl;
    if(grid[row][col-1].pCart != NULL){
        cout << "Collision at " << col-1 << "," << row << endl;
        grid[row][col].pCart->alive = false;
        grid[row][col-1].pCart->alive = false;
        grid[row][col].pCart = NULL;
        grid[row][col-1].pCart = NULL;
        return true;
    } else{
        track &t = grid[row][col];
        grid[row][col-1].pCart = t.pCart;
        t.pCart->hasMoved = true;
        t.pCart->row = row;
        t.pCart->col = col-1;
        t.pCart = NULL;
        return true;
    }
}

bool moveRight(vector< vector<track> >& grid, int row, int col){
    //cout << "Cart at " << col << "," << row << " moving right..." << endl;
    if(grid[row][col+1].pCart != NULL){
        cout << "Collision at " << col+1 << "," << row << endl;
        grid[row][col].pCart->alive = false;
        grid[row][col+1].pCart->alive = false;
        grid[row][col].pCart = NULL;
        grid[row][col+1].pCart = NULL;
        return true;
    } else{
        track &t = grid[row][col];
        grid[row][col+1].pCart = t.pCart;
        t.pCart->hasMoved = true;
        t.pCart->row = row;
        t.pCart->col = col+1;
        t.pCart = NULL;
        return true;
    }
}

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
    
    int numRows = input.size();
    int numCols = input[0].size();
    cout << "Grid size: " << numCols <<" cols, "<<numRows<<" rows"<<endl;

    // Build the track and cart state
    //track grid[numCols][numRows];
    vector< vector<track> > grid;
    vector<cart*> carts;

    for(int row = 0; row < numRows; row++){
        vector<track> tempRow;
        track tempTrack;
        string curRow = input[row];
        for(int col = 0; col < curRow.size(); col++){
            char s = curRow[col];
            // Check for a cart
            if(s == '<'){
                cart* c = new cart;
                c->dir = '<';
                c->hasMoved = false;
                c->row = row;
                c->col = col;
                c->alive = true;
                tempTrack.type = '-';
                tempTrack.pCart = c;
                carts.push_back(c);
            } else if(s == '>'){
                cart* c = new cart;
                c->dir = '>';
                c->hasMoved = false;
                c->row = row;
                c->col = col;
                c->alive = true;
                tempTrack.type = '-';
                tempTrack.pCart = c;
                carts.push_back(c);
            } else if(s == '^'){
                cart* c = new cart;
                c->dir = '^';
                c->hasMoved = false;
                c->row = row;
                c->col = col;
                c->alive = true;
                tempTrack.type = '|';
                tempTrack.pCart = c;
                carts.push_back(c);
            } else if(s == 'v'){
                cart* c = new cart;
                c->dir = 'v';
                c->hasMoved = false;
                c->row = row;
                c->col = col;
                c->alive = true;
                tempTrack.type = '|';
                tempTrack.pCart = c;
                carts.push_back(c);
            } else{ // Not a cart
                tempTrack.type = s;
                tempTrack.pCart = NULL;
            }
             tempRow.push_back(tempTrack);
        }
       grid.push_back(tempRow);
    }

    // Let the cart races begin!
    int numTicks = 0;
    while(true){

        cout << "Ticks: " << numTicks << endl;
        
        //printMine(grid);

        //string foo;
        //getline(cin, foo);

        numTicks++;

        for(int row=0; row < numRows; row++){
            for(int col = 0; col < numCols; col++){
                if(grid[row][col].pCart != NULL && !grid[row][col].pCart->hasMoved){
                    track t = grid[row][col];
                    switch (t.type){
                        case '-':
                            if(t.pCart->dir == '>'){
                                if(!moveRight(grid, row, col)) return 0;
                            } else { // dir == <
                                if(!moveLeft(grid, row, col)) return 0;
                            }
                            break;
                        case '|':
                            if(t.pCart->dir == '^'){
                                if(!moveUp(grid, row, col)) return 0;
                            } else { // dir == v
                                if(!moveDown(grid, row, col)) return 0;
                            }
                            break;
                        case '/': 
                            if(t.pCart->dir == '^'){
                                // Change direction in curve...
                                t.pCart->dir = '>';
                                if(!moveRight(grid, row, col)) return 0;
                            } 
                            else if(t.pCart->dir == 'v'){
                                // Change direction in curve...
                                t.pCart->dir = '<';
                                if(!moveLeft(grid, row, col)) return 0;
                            }
                            else if(t.pCart->dir == '>'){
                                // Change direction in curve...
                                t.pCart->dir = '^';
                                if(!moveUp(grid, row, col)) return 0;
                            }
                            else if(t.pCart->dir == '<'){
                                // Change direction in curve...
                                t.pCart->dir = 'v';
                                if(!moveDown(grid, row, col)) return 0;
                            }
                            else{
                                cout << "Impossible curve state /" << endl;
                                return -1;
                            }
                            break;
                        case '\\': 
                            if(t.pCart->dir == '^'){
                                // Change direction in curve...
                                t.pCart->dir = '<';
                                if(!moveLeft(grid, row, col)) return 0;
                            } 
                            else if(t.pCart->dir == 'v'){
                                // Change direction in curve...
                                t.pCart->dir = '>';
                                if(!moveRight(grid, row, col)) return 0;
                            }
                            else if(t.pCart->dir == '>'){
                                // Change direction in curve...
                                t.pCart->dir = 'v';
                                if(!moveDown(grid, row, col)) return 0;
                            }
                            else if(t.pCart->dir == '<'){
                                // Change direction in curve...
                                t.pCart->dir = '^';
                                if(!moveUp(grid, row, col)) return 0;
                            }
                            else{
                                cout << "Impossible curve state \\" << endl;
                                return -1;
                            }

                            break;
                        case '+':
                            switch (t.pCart->nextInt){
                                case 'L':
                                    t.pCart->nextInt = 'S';
                                    if(t.pCart->dir == '>'){
                                        t.pCart->dir = '^';
                                        if(!moveUp(grid, row, col)) return 0;
                                    }
                                    else if(t.pCart->dir == '<'){
                                        t.pCart->dir = 'v';
                                        if(!moveDown(grid, row, col)) return 0;
                                    }
                                    else if(t.pCart->dir == '^'){
                                        t.pCart->dir = '<';
                                        if(!moveLeft(grid, row, col)) return 0;
                                    }
                                    else{ // v
                                        t.pCart->dir = '>';
                                        if(!moveRight(grid, row, col)) return 0;
                                    }
                                    break;
                                case 'S':
                                    t.pCart->nextInt = 'R';
                                    // no change in direction...
                                    switch (t.pCart->dir){
                                        case '>':
                                            if(!moveRight(grid, row, col)) return 0;
                                            break;
                                        case '<':
                                            if(!moveLeft(grid, row, col)) return 0;
                                            break;
                                        case '^':
                                            if(!moveUp(grid, row, col)) return 0;
                                            break;
                                        case 'v':
                                            if(!moveDown(grid, row, col)) return 0;
                                            break;
                                        default:
                                            cout << "Impossible move straight through intersection" << endl;
                                            return -1;
                                    }
                                    break;
                                case 'R':
                                    t.pCart->nextInt = 'L';
                                    if(t.pCart->dir == '>'){
                                        t.pCart->dir = 'v';
                                        if(!moveDown(grid, row, col)) return 0;
                                    }
                                    else if(t.pCart->dir == '<'){
                                        t.pCart->dir = '^';
                                        if(!moveUp(grid, row, col)) return 0;
                                    }
                                    else if(t.pCart->dir == '^'){
                                        t.pCart->dir = '>';
                                        if(!moveRight(grid, row, col)) return 0;
                                    }
                                    else{ // v
                                        t.pCart->dir = '<';
                                        if(!moveLeft(grid, row, col)) return 0;
                                    }
                                    break;
                                default:
                                    cout << "Impossible + state" << endl;
                                    return -1;
                            }
                            break;
                        default:
                            cout << "Bogus track! " << t.type << endl;
                            return -1;
                    }
                }
            }
        }


        //Reset carts
        int numCarts = 0;
        int col, row;
        for(auto cart : carts){
            if(cart->alive){
                numCarts++;
                col = cart->col;
                row = cart->row;
                cart->hasMoved = false;
            }
        }

        if(numCarts == 1){
            cout << "Final cart: " << col << "," << row << endl;
            return 0;
        }
    }
}
