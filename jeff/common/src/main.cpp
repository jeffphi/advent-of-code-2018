#include <iostream>

char const*array();

int Foo()
{
    std::cout << array() << std::endl;
    return 0;
}
char const*array()
{
    return "Hello world from a static lib!";
}
