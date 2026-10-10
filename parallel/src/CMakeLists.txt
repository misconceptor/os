add_executable(baseline baseline.cpp)
target_compile_options(baseline PRIVATE -O2)

add_executable(loops loops.cpp)
target_compile_options(loops PRIVATE -O2)
add_executable(baseline_block baseline_block.cpp)
target_compile_options(baseline_block PRIVATE -O2)

add_executable(baseline_block_avx baseline_block_avx.cpp)
target_compile_options(baseline_block_avx PRIVATE -O3 -march=native)

add_executable(helper helper.cpp)

# add_executable(main main.cpp)
# target_compile_options(main PRIVATE -O3 -march=native)
