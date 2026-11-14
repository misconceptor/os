cmake_minimum_required(VERSION 3.16)
project(decomp CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP REQUIRED)

add_executable(helper helper.cpp)
target_compile_options(helper PRIVATE -O2)

add_executable(stupid stupid.cpp utils.h)
target_compile_options(stupid PRIVATE -O2)

add_executable(dd dd.cpp dd.h)
target_compile_options(dd PRIVATE -O2)

add_executable(par par.cpp utils.h)
target_compile_options(par PRIVATE -O2)
target_link_libraries(par PRIVATE OpenMP::OpenMP_CXX)

add_executable(par_dd par_dd.cpp dd.h)
target_compile_options(par_dd PRIVATE -O2)
target_link_libraries(par_dd PRIVATE OpenMP::OpenMP_CXX)