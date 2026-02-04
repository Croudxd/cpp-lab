cmake_minimum_required(VERSION 3.10)
project(cpp-lab)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

include_directories(include)

enable_testing()


#BENCHMARKS:
add_executable(benchmark_vector benchmarks/benchmark_vector.cpp)


#TEST:
add_executable(test_vector tests/test_vector.cpp) 
add_executable(test_shared_ptr tests/test_shared_ptr.cpp) 

add_test(NAME VectorTest COMMAND test_vector)
add_test(NAME SharedPtrTest COMMAND test_shared_ptr)
