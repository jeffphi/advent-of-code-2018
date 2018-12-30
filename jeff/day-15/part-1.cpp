#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "jefflib.h" 
#include <queue>  
#include <ncurses.h>

using namespace std;

enum UnitKind { elf, goblin };
enum CellKind { open, wall };
enum SeachDir { up, left, right, down };

struct Unit {
    UnitKind kind;
    int hp = 200;
    int pwr = 3;
    bool hasMoved = false;
};

struct Cell {
    CellKind kind;
    Unit *pUnit = NULL;
    int row;
    int col;
    bool marked;
    int searchDist;
};

#define ELF_PAIR    1
#define CYAN_PAIR   2
#define RED_PAIR    3
#define GOBLIN_PAIR 4

void printDebug(vector< vector<Cell> >& grid, string msg){

    mvprintw(grid.size()+2, 0, msg.c_str());
}

void printCave(vector< vector<Cell> >& grid, int round){
    clear();
     for(int r = 0; r < grid.size(); r++){
        stringstream rowStream;
        for(int c = 0; c < grid[0].size(); c++){
            if(grid[r][c].pUnit != NULL){
                if(grid[r][c].pUnit->kind == elf) {
                    //attron(A_BOLD);
                    attron(COLOR_PAIR(ELF_PAIR));
                    mvaddch(r,c,'E');
                    attroff(COLOR_PAIR(ELF_PAIR));
                    //attroff(A_BOLD);
                    rowStream << "E(" << grid[r][c].pUnit->hp << ") ";
                }
                if(grid[r][c].pUnit->kind == goblin) {
                    attron(COLOR_PAIR(GOBLIN_PAIR));
                    mvaddch(r,c,'G');
                    attroff(COLOR_PAIR(GOBLIN_PAIR));
                    rowStream << "G(" << grid[r][c].pUnit->hp << ") ";
                }
            }
            else{
                if(grid[r][c].kind == open) mvaddch(r,c,'.');
                if(grid[r][c].kind == wall) mvaddch(r,c,'#');
            }
        }
        mvprintw(r, grid[0].size()+1, rowStream.str().c_str());
    }
    mvprintw(grid.size()+1, 0,"Number of Rounds:  %d", round);
}

bool Done(vector< vector<Cell> >& grid, int rounds){
    int numElves = 0;
    int numGoblins = 0;
    int totalHP = 0;

    for(int r = 0; r < grid.size(); r++){
        for(int c = 0; c < grid[0].size(); c++){
            if(grid[r][c].pUnit != NULL){
                if(grid[r][c].pUnit->kind == elf){
                    numElves++;
                }
                else{
                    numGoblins++;
                }
                totalHP += grid[r][c].pUnit->hp;
            }
        }
    }

    if(numElves == 0 || numGoblins == 0){
        stringstream ss;

        ss << "Done! Full rounds: " << rounds << ", Remaining HP: " << totalHP << ", Score: " << (rounds*totalHP);
        printDebug(grid, ss.str());
        return true;
    }

    return false;
}

void CheckIfDead(vector< vector<Cell> >& grid, int r, int c){
    if(grid[r][c].pUnit->hp <= 0){
        grid[r][c].pUnit = NULL;
    }
}

bool AttackedEnemy(vector< vector<Cell> >& grid, int r, int c){
    Unit *curUnit = grid[r][c].pUnit;

    // Look around in reading order, though choose to attack enemy with lowest hp
    int upHP = INT_MAX;
    int leftHP = INT_MAX;
    int rightHP = INT_MAX;
    int downHP = INT_MAX;

    if(grid[r-1][c].pUnit != NULL && grid[r-1][c].pUnit->kind != curUnit->kind){
        upHP = grid[r-1][c].pUnit->hp;
    }
    if(grid[r][c-1].pUnit != NULL && grid[r][c-1].pUnit->kind != curUnit->kind){
        leftHP = grid[r][c-1].pUnit->hp;
    }
    if(grid[r][c+1].pUnit != NULL && grid[r][c+1].pUnit->kind != curUnit->kind){
        rightHP = grid[r][c+1].pUnit->hp;
    }
    if(grid[r+1][c].pUnit != NULL && grid[r+1][c].pUnit->kind != curUnit->kind){
        downHP = grid[r+1][c].pUnit->hp;
    }

    // Attack!
    if(upHP < INT_MAX && upHP <= leftHP && upHP <= rightHP && upHP <=downHP){
        grid[r-1][c].pUnit->hp -= curUnit->pwr;
        CheckIfDead(grid, r-1, c);
        return true;
    }
    if(leftHP < INT_MAX && leftHP <= rightHP && leftHP <= downHP){
        grid[r][c-1].pUnit->hp -= curUnit->pwr;
        CheckIfDead(grid, r, c-1);
        return true;
    }
    if(rightHP < INT_MAX && rightHP <= downHP){
        grid[r][c+1].pUnit->hp -= curUnit->pwr;
        CheckIfDead(grid, r, c+1);
        return true;
    }
    if(downHP < INT_MAX){
        grid[r+1][c].pUnit->hp -= curUnit->pwr;
        CheckIfDead(grid, r+1, c);
        return true;
    }

    return false;
}

void ClearMoved(vector< vector<Cell> >& grid){
    for(int r = 0; r < grid.size(); r++){
        for(int c = 0; c < grid[0].size(); c++){
            if(grid[r][c].pUnit != NULL){
                grid[r][c].pUnit->hasMoved = false;
            }
        }
    }
}

void ClearMarks(vector< vector<Cell> >& grid){
    for(int r = 0; r < grid.size(); r++){
        for(int c = 0; c < grid[0].size(); c++){
    
            grid[r][c].marked = false;
        }
    }
}

int GetClosest(vector< vector<Cell> >& grid, queue<Cell>& searchQueue, UnitKind kind){
    int maxRow = grid.size();
    int maxCol = grid[0].size();

    while(searchQueue.size() > 0){
        Cell curCell = searchQueue.front();
        searchQueue.pop();

        // Check if this is a legit cell
        if(curCell.pUnit != NULL || curCell.kind != open){
            continue;
        }

        int r = curCell.row;
        int c = curCell.col;
        
        // If next to an ememy, done.
        if(r>0 && grid[r-1][c].pUnit != NULL && grid[r-1][c].pUnit->kind != kind){
            return curCell.searchDist;
        }
        if(c>0 && grid[r][c-1].pUnit != NULL && grid[r][c-1].pUnit->kind != kind){
            return curCell.searchDist;
        }
        if(c<maxCol &&grid[r][c+1].pUnit != NULL && grid[r][c+1].pUnit->kind != kind){
            return curCell.searchDist;
        }
        if(r<maxRow && grid[r+1][c].pUnit != NULL && grid[r+1][c].pUnit->kind != kind){
            return curCell.searchDist;
        }

        // Otherwise, keep searching (in reading order)
        if(!grid[r-1][c].marked){
            grid[r-1][c].marked = true;
            grid[r-1][c].searchDist = curCell.searchDist + 1;
            searchQueue.push(grid[r-1][c]);
        }
        if(!grid[r][c-1].marked){
            grid[r][c-1].marked = true;
            grid[r][c-1].searchDist = curCell.searchDist + 1;
            searchQueue.push(grid[r][c-1]);
        }
        if(!grid[r][c+1].marked){
            grid[r][c+1].marked = true;
            grid[r][c+1].searchDist = curCell.searchDist + 1;
            searchQueue.push(grid[r][c+1]);
        }
        if(!grid[r+1][c].marked){
            grid[r+1][c].marked = true;
            grid[r+1][c].searchDist = curCell.searchDist + 1;
            searchQueue.push(grid[r+1][c]);
        }
    }

    return INT_MAX;
}

bool Moved(vector< vector<Cell> >& grid, int r, int c, int& newRow, int&  newCol){

    Unit *curUnit = grid[r][c].pUnit;
    curUnit->hasMoved = true;
    UnitKind kind = curUnit->kind;

    queue<Cell> searchQueueUp;
    grid[r-1][c].searchDist = 1;
    grid[r-1][c].marked = true;
    searchQueueUp.push(grid[r-1][c]);
    int distUp = GetClosest(grid, searchQueueUp, kind);
    ClearMarks(grid);

    queue<Cell> searchQueueLeft;
    grid[r][c-1].searchDist = 1;
    grid[r][c-1].marked = true;
    searchQueueLeft.push(grid[r][c-1]);
    int distLeft = GetClosest(grid, searchQueueLeft, kind);
    ClearMarks(grid);

    queue<Cell> searchQueueRight;
    grid[r][c+1].searchDist = 1;
    grid[r][c+1].marked = true;
    searchQueueRight.push(grid[r][c+1]);
    int distRight = GetClosest(grid, searchQueueRight, kind);
    ClearMarks(grid);

    queue<Cell> searchQueueDown;
    grid[r+1][c].searchDist = 1;
    grid[r+1][c].marked = true;
    searchQueueDown.push(grid[r+1][c]);
    int distDown = GetClosest(grid, searchQueueDown, kind);
    ClearMarks(grid);

    if(distUp <= distLeft && distUp <= distRight && distUp <= distDown && distUp < INT_MAX){
        grid[r][c].pUnit = NULL;
        grid[r-1][c].pUnit = curUnit;
        newRow = r-1;
        newCol = c;
        return true;
    }
    else if(distLeft <= distRight && distLeft <= distDown && distLeft < INT_MAX){
        grid[r][c].pUnit = NULL;
        grid[r][c-1].pUnit = curUnit;
        newRow = r;
        newCol = c-1;
        return true;
    }
    else if(distRight <= distDown && distRight < INT_MAX){
        grid[r][c].pUnit = NULL;
        grid[r][c+1].pUnit = curUnit;
        newRow = r;
        newCol = c+1;
        return true;
    }
    else if(distDown < INT_MAX){
        grid[r][c].pUnit = NULL;
        grid[r+1][c].pUnit = curUnit;
        newRow = r+1;
        newCol = c;
        return true;
    }

    return false;
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

    initscr();
    cbreak();
    noecho();
    curs_set(0);
    start_color();
    init_pair(ELF_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(CYAN_PAIR, COLOR_CYAN, COLOR_BLACK);
    init_pair(RED_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(GOBLIN_PAIR, COLOR_GREEN, COLOR_BLACK);
    clear();
    
    int numRows = input.size();

    vector< vector<Cell> > grid;

    for(int row = 0; row < numRows; row++){
        vector<Cell> tempRow;
        Cell tempCell;
        string curRow = input[row];
        for(int col = 0; col < curRow.size(); col++){
            tempCell.row = row;
            tempCell.col = col;
            tempCell.marked = false;

            char s = curRow[col];
            if(s == 'E'){
                Unit* u = new Unit;
                u->kind = elf;
                u->hasMoved = false;
                tempCell.pUnit = u;
                tempCell.kind = open;
            } else if(s == 'G'){
                Unit* u = new Unit;
                u->kind = goblin;
                u->hasMoved = false;
                tempCell.pUnit = u;
                tempCell.kind = open;
            } else if(s == '#'){
                tempCell.pUnit = NULL;
                tempCell.kind = wall;
            } else if(s == '.'){
                tempCell.pUnit = NULL;
                tempCell.kind = open;
            }
             tempRow.push_back(tempCell);
        }
       grid.push_back(tempRow);
    }

    // Let the battle begin!
    int numRounds = 0;
    while(true){
        printCave(grid, numRounds);

        // Process each Unit in reading order
        for(int r = 0; r < grid.size(); r++){
            for(int c = 0; c < grid[0].size(); c++){
                if(grid[r][c].pUnit != NULL){
                    // Are we right next to an enemy?
                    if(!grid[r][c].pUnit->hasMoved && AttackedEnemy(grid, r, c)){
                        continue;
                    }
                    // Otherwise, move and then try to attack
                    int newRow;
                    int newCol;
                    if(!grid[r][c].pUnit->hasMoved && Moved(grid, r, c, newRow, newCol)){
                        AttackedEnemy(grid, newRow, newCol);
                    }
                }
            }
        }
        numRounds++;
        if(getch() == 'q') break;
        ClearMoved(grid);
        if(Done(grid, numRounds-1)) break;
    }

    getch();
    endwin();
    return 0;
}
