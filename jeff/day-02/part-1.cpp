#include <fstream>
#include <iostream>
#include <string>
#include <map>

using namespace std;
int main() 
{ 
    int countOfTwo = 0;
    int countOfThree = 0;

    ifstream file("input");
    string str; 
    while (getline(file, str))
    {
        map<char, int> tracker;

        //Add chars, accumulating
        for(char& c : str) {
            tracker[c] = tracker[c]+1;
        }

        //Check for a pair
        for (pair<char, int> item : tracker)
        {
            if (item.second == 2){
                countOfTwo++;
                break;
            }
        }
        //Check for three
        for (pair<char, int> item : tracker)
        {
            if (item.second == 3){
                countOfThree++;
                break;
            }
        }
 
    }
    cout << "Checksum: " << countOfTwo * countOfThree << endl;
}
