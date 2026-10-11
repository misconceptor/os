add_executable(baseline baseline.cpp)
target_compile_options(baseline PRIVATE -O2)

add_executable(loops loops.cpp)
target_compile_options(loops PRIVATE -O2)

add_executable(block block.cpp)
target_compile_options(block PRIVATE -O2)

add_executable(block_avx block_avx.cpp)
target_compile_options(block_avx PRIVATE -O3 -march=native)

add_executable(helper helper.cpp)
add_executable(helper_par helper_par.cpp)

find_package(OpenMP REQUIRED)

add_executable(par1 par1.cpp)
target_compile_options(par1 PRIVATE -O2)
target_link_libraries(par1 PRIVATE OpenMP::OpenMP_CXX)

add_executable(par1_avx par1_avx.cpp)
target_compile_options(par1_avx PRIVATE -O3 -march=native)
target_link_libraries(par1_avx PRIVATE OpenMP::OpenMP_CXX)

add_executable(par2 par2.cpp)
target_compile_options(par2 PRIVATE -O2)
target_link_libraries(par2 PRIVATE OpenMP::OpenMP_CXX)

add_executable(par2_avx par2_avx.cpp)
target_compile_options(par2_avx PRIVATE -O3 -march=native)
target_link_libraries(par2_avx PRIVATE OpenMP::OpenMP_CXX)

