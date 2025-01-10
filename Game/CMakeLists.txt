cmake_minimum_required(VERSION 3.30)
project(Game)

set(CMAKE_CXX_STANDARD 26)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_subdirectory(../Engine/Engine Engine_build)
include_directories(../Engine/Engine)

add_executable(Game src/main.cpp)

target_include_directories(Game PRIVATE ../Engine/Engine)
target_link_libraries(Game PRIVATE Engine)