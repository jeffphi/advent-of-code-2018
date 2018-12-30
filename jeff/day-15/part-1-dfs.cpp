#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "jefflib.h" 
#include <unordered_map>
#include <ncurses.h>

using namespace std;

enum UnitKind { elf, goblin };
enum CellKind { open, wall };

struct Unit {
    UnitKind kind;
    int hp = 200;
    int pwr = 3;
    bool hasMoved = false;
};

struct Cell {
    CellKind kind;
    Unit *pUnit = NULL;
};

#define ELF_PAIR    1
#define CYAN_PAIR   2
#define RED_PAIR    3
#define GOBLIN_PAIR 4

// Globals!
int current_best;

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1,T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);

        // Mainly for demonstration purposes, i.e. works but is overly simple
        // In the real world, use sth. like boost.hash_combine
        return h1 ^ h2;  
    }
};

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
    // Look around in reading order
    if(grid[r-1][c].pUnit != NULL && grid[r-1][c].pUnit->kind != curUnit->kind){
        grid[r-1][c].pUnit->hp -= curUnit->pwr;
        CheckIfDead(grid, r-1, c);
        return true;
    }
    if(grid[r][c-1].pUnit != NULL && grid[r][c-1].pUnit->kind != curUnit->kind){
        grid[r][c-1].pUnit->hp -= curUnit->pwr;
        CheckIfDead(grid, r, c-1);
        return true;
    }
    if(grid[r][c+1].pUnit != NULL && grid[r][c+1].pUnit->kind != curUnit->kind){
        grid[r][c+1].pUnit->hp -= curUnit->pwr;
        CheckIfDead(grid, r, c+1);
        return true;
    }
    if(grid[r+1][c].pUnit != NULL && grid[r+1][c].pUnit->kind != curUnit->kind){
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

int GetClosest(vector< vector<Cell> >& grid, UnitKind kind, int r, int c, unordered_map<pair<int,int>, int, pair_hash> list){
    int maxRow = grid.size();
    int maxCol = grid[0].size();

    // Avoid excessive searching...
    if(list.size() > current_best){
        return INT_MAX;
    }

    // Check if this is a legit cell
    if(grid[r][c].pUnit != NULL || grid[r][c].kind != open){
        return INT_MAX;
    }

    //printCave(grid, -1);
    //getch();

    //printDebug(grid, ss.str());
    //stringstream ss;
    //ss << "Looking down: pUnit:"<<grid[r+1][c].pUnit<<", myKind:"<<kind;
    //printDebug(grid, ss.str());
    //getch();

    /*
    stringstream ss;
    ss << "GetClosest r:"<<r<<" c:"<<c<<" count:"<<count;
    printDebug(grid, ss.str());
    getch();
    */
    
    // If next to an ememy, done.
    if(r>0 && grid[r-1][c].pUnit != NULL && grid[r-1][c].pUnit->kind != kind){
        current_best = list.size();
        return list.size();
    }

    if(c>0 && grid[r][c-1].pUnit != NULL && grid[r][c-1].pUnit->kind != kind){
        current_best = list.size();
        return list.size();
    }
    if(c<maxCol &&grid[r][c+1].pUnit != NULL && grid[r][c+1].pUnit->kind != kind){
        current_best = list.size();
        return list.size();
    }
    if(r<maxRow && grid[r+1][c].pUnit != NULL && grid[r+1][c].pUnit->kind != kind){
        current_best = list.size();
        return list.size();
    }

    // If we can move in a direction, return the lowest value...
    int u = INT_MAX;
    int l = INT_MAX;
    int rt = INT_MAX;
    int d = INT_MAX;

    pair<int,int> tempPair = make_pair(r-1, c);
    bool marked = list.find(tempPair) != list.end();
    
    if(r>0 && grid[r-1][c].kind == open && grid[r-1][c].pUnit == NULL && !marked){
        unordered_map<pair<int,int>, int, pair_hash> newList = list;
        newList[make_pair(r-1,c)] = 0;
        u = GetClosest(grid, kind, r-1, c, newList);
    }

    tempPair = make_pair(r, c-1);
    marked = list.find(tempPair) != list.end();
    if(c>0 &&grid[r][c-1].kind == open && grid[r][c-1].pUnit == NULL && !marked){
        unordered_map<pair<int,int>, int, pair_hash> newList = list;
        newList[make_pair(r,c-1)];
        l = GetClosest(grid, kind, r, c-1, newList);
    }

    tempPair = make_pair(r, c+1);
    marked = list.find(tempPair) != list.end();
    if(c<maxCol && grid[r][c+1].kind == open && grid[r][c+1].pUnit == NULL && !marked){
        unordered_map<pair<int,int>, int, pair_hash> newList = list;
        newList[make_pair(r,c+1)];
        rt = GetClosest(grid, kind, r, c+1, newList);
    }

    tempPair = make_pair(r+1, c);
    marked = list.find(tempPair) != list.end();
    if(r<maxRow && grid[r+1][c].kind == open && grid[r+1][c].pUnit == NULL && !marked){
        unordered_map<pair<int,int>, int, pair_hash> newList = list;
        newList[make_pair(r+1,c)];
        d = GetClosest(grid, kind, r+1, c, newList);
    }

    // Return the lowest score
    if(u<=rt && u<=l && u<=d) return u;
    if(l<=rt && l<=d ) return l;
    if(rt <= d) return rt;
    return d;
}

bool Moved(vector< vector<Cell> >& grid, int r, int c, int& newRow, int&  newCol){
    /*
    stringstream ss;
    ss << "Moved...";
    printDebug(grid, ss.str());
    getch();
    */

    Unit *curUnit = grid[r][c].pUnit;
    curUnit->hasMoved = true;
    UnitKind myKind = curUnit->kind;

    unordered_map<pair<int,int>, int, pair_hash> upList;
    upList[make_pair(r,c)] = 0;

    unordered_map<pair<int,int>, int, pair_hash> leftList = upList;
    unordered_map<pair<int,int>, int, pair_hash> rightList = leftList;
    unordered_map<pair<int,int>, int, pair_hash> downList = rightList;

    current_best = INT_MAX;
    int distUp      = GetClosest(grid, myKind, r-1, c,      upList);

    current_best = INT_MAX;
    int distLeft    = GetClosest(grid, myKind, r,   c-1,    leftList);

    current_best = INT_MAX;
    int distRight   = GetClosest(grid, myKind, r,   c+1,    rightList);

    current_best = INT_MAX;
    int distDown    = GetClosest(grid, myKind, r+1, c,      downList);
    
    //stringstream ss;
    //ss << "In Moved: r: "<<r<<", c:"<<c<<" u: "<<distUp<<" l:"<<distLeft<<" r:"<<distRight<<" d:"<<distDown;
    //printDebug(grid, ss.str());
    //getch();
    

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
        //if(getch() == 'q') break;
        ClearMoved(grid);
        if(Done(grid, numRounds-1)) break;
    }

    getch();
    endwin();
    return 0;
}
