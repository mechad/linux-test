/*
 * @file: stdmove.cpp
 * @create time: 2018-12-20 11:32:11
 * @last modified: 2018-12-20 11:32:11
 * @description:
 */
#include <iostream>
#include <vector>

using namespace std;

std::vector<std::string> v;

void TestSTLObject()
{
    std::string str = "Hello";
 
    // uses the push_back(const T&) overload, which means
    // we'll incur the cost of copying str
    v.push_back(str);
    std::cout << "After copy, str is \"" << str << "\"\n";
 
    // uses the rvalue reference push_back(T&&) overload,
    // which means no strings will be copied; instead, the contents
    // of str will be moved into the vector.  This is less
    // expensive, but also means str might now be empty.
    v.push_back(std::move(str));
    std::cout << "After move, str is \"" << str << "\"\n";
 
    std::cout << "The contents of the vector are \"" << v[0]
                                         << "\", \"" << v[1] << "\"\n";
 
}

int main()
{
    TestSTLObject();
    std::cout << "-----------------------------" << endl;
    std::cout << "The contents of the vector are \"" << v[0]
                                         << "\", \"" << v[1] << "\"\n";
}
