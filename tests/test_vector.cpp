#include "ben/vector.hpp"
#include <iostream>
#include <cassert>

using namespace std;

int main ()
{
    ben::vector<int> v;
    assert(v.size() == 0);
    v.push_back(8);
    assert(v[0]==8);
    v.push_back(1);
    v.push_back(1);
    v.push_back(1);
    v.push_back(1);
    v.push_back(1);
    assert(v.size() == 6);
    assert(v[1] == 1);
    assert(v[1] == 1);
    assert(v[1] == 1);
    assert(v[1] == 1);
    assert(v[1] == 1);
    

    return 0;
}
