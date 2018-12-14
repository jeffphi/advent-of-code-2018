#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "jefflib.h" 
#include <boost/tokenizer.hpp>
#include <thread>
#include <chrono>
#include <ncurses.h>

using namespace std;
using namespace boost;

struct star{
    int x, y, dx, dy;
};

void PrintStars(vector<star>& stars, int rows, int cols, int maxX, int maxY, int minX, int minY, int time){
   
    // -----> Status <----- //
    mvprintw(0,0,"Screen Columns: %d",cols);
    mvprintw(1,0,"Screen Rows: %d",rows);
    mvprintw(2,0,"Nunber of Stars: %d",stars.size());
    mvprintw(4,0,"Time: %d",time);

    int miX = INT_MAX;
    int miY = INT_MAX;
    int maX = 0;
    int maY = 0;
    for(auto star : stars){
        int y = (rows*star.y)/maxY;
        int x = (cols*star.x)/maxX;
        mvaddch(y,x,'*');

        if(x<miX)miX=x;
        if(x>maX)maX=x;
        if(y>maY)maY=y;
        if(y<miY)miY=y;
    }
    mvprintw(3,0,"MinX %d, MinX %d, MaxX %d, MaxY %d", miX, miY, maX, maY);
    refresh();
}


void TranslateStars(int minX, int minY, vector<star>& stars, int& maxX, int& maxY){
    maxX = 0;
    maxY = 0;
    for(auto& star : stars){
        if(minX < 0){
            star.x += abs(minX);
        }
        if(minX > 0){
            star.x -= minX;
        }

        if(minY < 0){
            star.y += abs(minY);
        }
        if(minY > 0){
            star.y -= minY;
        }
        if(star.x>maxX) maxX = star.x;
        if(star.y>maxY) maxY = star.y;
    }
}

void UpdateStars(vector<star>& stars, int& maxX, int& maxY, int& minX, int& minY){
    
    // Update them
    for(auto& star : stars){
        star.x += star.dx;
        star.y += star.dy;
    }
    minX = INT_MAX;
    minY = INT_MAX;
    maxX = 0;
    maxY = 0;

    for(auto star : stars){
        if(star.x < minX) minX = star.x;
        if(star.x > maxX) maxX = star.x;
        if(star.y < minY) minY = star.y;
        if(star.y > maxY) maxY = star.y;
    }
    
    TranslateStars(minX, minY, stars, maxX, maxY);

}

int main()
{
    initscr();
    cbreak();
    noecho();
    clear();
    
    int row, col;
    getmaxyx(stdscr,row,col);

    vector<string> vect;
    GetStringInput(vect);
   
    // Example input line, nicely well-formed :)
    //  ..........10.....17.................36.39..
    //  position=<-31503, -52596> velocity=< 3,  5>
    
    vector<star> stars;
    for(auto line : vect){
        star tempStar;
        tempStar.x  = stoi(line.substr(10,6));
        tempStar.y  = stoi(line.substr(17, 7));
        tempStar.dx = stoi(line.substr(36, 2));
        tempStar.dy = stoi(line.substr(39, 3));
        stars.push_back(tempStar);
    }

    int time = 0;
    int maxX = 0;
    int maxY = 0;
    int minX = INT_MAX;
    int minY = INT_MAX;
    int delay = 0;
    for(;;){
        clear();
        time++;
        UpdateStars(stars, maxX, maxY, minX, minY);
        PrintStars(stars, row, col, maxX, maxY, minX, minY, time);
        this_thread::sleep_for(chrono::milliseconds(delay));
        if(time > 10000) delay = 10; //100
        if(time > 10500) delay = 100; //500
        if(time > 10550) delay = 500; //1000
        if(time == 10558) delay = 5000;
        if(time > 10558) delay = 500; 
        /*
        if(time == 10558){
            ofstream myfile;
            myfile.open ("out-558.csv");
            for(auto star : stars){
                myfile << star.x <<","<<star.y<<endl;
            }
            myfile << "Writing this to a file.\n";
            myfile.close();
            return 0;
        }
        */
    }
}


