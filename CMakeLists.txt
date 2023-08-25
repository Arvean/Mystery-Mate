cmake_minimum_required(VERSION 3.10)
project(ChessProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src)

# Assuming your main.cpp is in the src directory
add_executable(ChessProject src/main.cpp)
target_link_libraries(ChessProject PRIVATE chess_srcs)
target_include_directories(ChessProject PRIVATE include)

set(CMAKE_BUILD_TYPE Debug)

enable_testing()
add_subdirectory(tests)


