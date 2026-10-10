add_executable(main main.cpp)
add_executable(baseline baseline.cpp)

target_compile_options(main PRIVATE -O3 -march=native)
target_compile_options(baseline PRIVATE -O3 -march=native)
