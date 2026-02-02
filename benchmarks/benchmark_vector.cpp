#include <iostream>
#include <chrono>
#include <vector> 
#include "ben/vector.hpp"

int main()
{
    int count = 10000000;
    ben::vector<std::string> v;
    std::string str = "asdflkajsdflkjasd;lkfja;lskdj;laksjdfl;kjfkdjf;askdjf";

    v.reserve(10000000);
    // Start timer
    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < count; ++i) {
        v.push_back(str);
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> diff = end - start;
    
    std::cout << "Pushed " << count << " items in: " << diff.count() << " seconds" << std::endl;

    std::vector<std::string> vec;
    vec.reserve(10000000);

    auto startvec = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < count; ++i) {
        vec.push_back(str);
    }

    auto endvec = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> diffvec = endvec - startvec;
    
    std::cout << "Pushed " << count << " items in: " << diffvec.count() << " seconds" << std::endl;
    return 0;
}
