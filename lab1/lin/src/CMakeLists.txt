add_library(Number STATIC Number.cpp Number.h)
target_include_directories(Number PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(Vector SHARED Vector.cpp Vector.h)
target_include_directories(Vector PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(main main.cpp)

target_link_libraries(main PRIVATE Number Vector) 