cmake_minimum_required(VERSION 3.16)
project(decomp CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find_package(OpenMP REQUIRED)

add_executable(helper helper.cpp)
target_compile_options(helper PRIVATE -O2)

add_executable(main main.cpp)
target_compile_options(main PRIVATE -O2)