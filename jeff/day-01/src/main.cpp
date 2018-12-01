#include <fstream>
#include <iostream>
#include <string>

using namespace std;
int main() 
{ 
    ifstream file("input");
    string str; 
    int sum;

    while (getline(file, str))
    {
        sum += stoi(str);
    }

    cout << "Sum: " << sum << endl;
}
