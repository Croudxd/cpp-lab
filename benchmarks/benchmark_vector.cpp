#include <chrono>
#include "ben/vector.hpp"
#include <vector>
#include <string>
#include <iostream>

template <typename T, typename VecType>
void run_test(size_t count, const T& item, const std::string& label) 
{
    VecType v;
    
    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < count; ++i) 
    {
        v.push_back(item);
    }

    auto end = std::chrono::high_resolution_clock::now();
    
    if (v.size() > 0 && v[0] == item) 
    {
        std::chrono::duration<double> diff = end - start;
        std::cout << "[" << label << "] Time: " << diff.count() << "s" << std::endl;
    }
}

int main() 
{
    size_t n = 10000000;

    std::cout << "--- Testing Integers ---" << std::endl;
    run_test<int, ben::vector<int>>(n, 42, "Ben Vector (int)");
    run_test<int, std::vector<int>>(n, 42, "Std Vector (int)");

    std::cout << "--- Testing Strings ---" << std::endl;
    std::string heavy_str = "This is a reasonably long string to trigger heap allocation";
    run_test<std::string, ben::vector<std::string>>(n, heavy_str, "Ben Vector (string)");
    run_test<std::string, std::vector<std::string>>(n, heavy_str, "Std Vector (string)");

    return 0;
}
