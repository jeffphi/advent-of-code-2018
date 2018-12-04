#include <iostream>
#include <vector>
#include "jefflib.h" 

using namespace std;

int main()
{
	cout << "Hello World!" << endl;
    cout << "Jeff here..." << endl;

    vector<string> vect;
    if(GetStringInput(vect)){
        cout << "Got data!" << endl;
        for (vector<string>::const_iterator i = vect.begin(); i != vect.end(); ++i){
            cout << *i << ' ';
        }
        cout << endl;
    }
    else {
        cout << "Failed to read input :( " << cout;
    }
    return Foo();
}
