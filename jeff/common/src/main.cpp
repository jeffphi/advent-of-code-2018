 #include <fstream>
 #include <iostream>
 #include <string>
 #include <vector>

//namespace JeffLib {

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

    bool GetStringInput(std::vector<std::string> &vectStrings) 
    {
        std::ifstream file("input");
        if(!file)     
        {
            std::cout << "Error opening input file" << std::endl;
            return false;
        }

        std::string str; 
        while (getline(file, str))
        {
            vectStrings.push_back(str);
        }

        return true;
    }
//}
