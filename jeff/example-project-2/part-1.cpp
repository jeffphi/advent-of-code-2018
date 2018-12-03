#include <iostream>
#include <vector>
#include "jefflib.h" 
#include <boost/lambda/lambda.hpp>

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

    // Testing boost
    using namespace boost::lambda;
    typedef std::istream_iterator<int> in;

    std::for_each(
        in(std::cin), in(), std::cout << (_1 * 3) << " " );

    return Foo();
}
