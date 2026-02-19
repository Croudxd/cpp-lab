cmake_minimum_required(VERSION 3.10)
project(cpp-lab)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

include_directories(include)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
  benchmark
  URL https://github.com/google/benchmark/archive/refs/tags/v1.8.3.zip
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(benchmark)

find_package(Threads REQUIRED)
enable_testing()

include(GoogleTest)

#BENCHMARKS:
add_executable(benchmark_vector benchmarks/benchmark_vector.cpp)

#TEST:
add_executable(test_vector tests/test_vector.cpp) 
add_executable(test_shared_ptr tests/test_shared_ptr.cpp) 

target_link_libraries(test_shared_ptr GTest::gtest_main Threads::Threads)
gtest_discover_tests(test_shared_ptr)

target_link_libraries(test_vector GTest::gtest_main Threads::Threads)
gtest_discover_tests(test_vector)
