cmake_minimum_required(VERSION 3.30)
project(Test)

set(CMAKE_CXX_STANDARD 26)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_subdirectory(../Engine Engine_build)
include_directories(../Engine)

include_directories(../Test)

add_executable(Test
        StopCMakeError.cpp)

target_include_directories(Test PRIVATE ../Engine)
target_link_libraries(Test PRIVATE Engine)