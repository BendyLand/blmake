#include <iostream>
#include "utils.hpp"

// g++ -std=c++20 *.cpp -Iinclude -o build/main
using std::string; using std::cout; using std::endl;

int main()
{
    cout << "Hello change!" << endl;
    string test1 = "test";
    string test2 = "#include <stdio.h>";
    string test3 = "#define None 0";
    string test4 = "#include \"utils.h\"";
    string test5 = "another test";

    bool res1 = is_include(test1);
    bool res2 = is_include(test2);
    bool res3 = is_include(test3);
    bool res4 = is_include(test4);
    bool res5 = is_include(test5);

    cout << "Result: " << res1 << endl;
    cout << "Result: " << res2 << endl;
    cout << "Result: " << res3 << endl;
    cout << "Result: " << res4 << endl;
    cout << "Result: " << res5 << endl;

    return 0;
}