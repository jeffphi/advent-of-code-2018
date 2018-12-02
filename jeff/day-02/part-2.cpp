#include <fstream>
#include <iostream>
#include <string>
#include <map>

using namespace std;
int main() 
{ 
    ifstream file("input");
    string str1, str2; 
    while (getline(file, str1))
    {
        ifstream file2("input");
        while (getline(file2, str2))
        {
            int numDiffs = 0;
            for(string::size_type i = 0; i < str1.size(); ++i) {
                if(str1[i] != str2[i]){
                    numDiffs++;
                }
            }
            if(numDiffs == 1){
                cout << "Found it! " << str1 << ", " << str2 << endl;
                return 0;
            }
       }
    }
}
