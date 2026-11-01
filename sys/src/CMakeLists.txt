cmake_minimum_required(VERSION 3.16)
project(sys CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP REQUIRED)

add_executable(helper helper.cpp)
target_compile_options(helper PRIVATE -O2)

add_executable(stupid stupid.cpp utils.h)
target_compile_options(stupid PRIVATE -O2)

add_executable(pivot pivot.cpp utils.h)
target_compile_options(pivot PRIVATE -O2)

add_executable(stupid_par stupid_par.cpp utils.h)
target_compile_options(stupid_par PRIVATE -O2)
target_link_libraries(stupid_par PRIVATE OpenMP::OpenMP_CXX)

add_executable(pivot_par pivot_par.cpp utils.h)
target_compile_options(pivot_par PRIVATE -O2)
target_link_libraries(pivot_par PRIVATE OpenMP::OpenMP_CXX)