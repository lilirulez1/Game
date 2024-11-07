cmake_minimum_required(VERSION 3.29)
project(Game)

set(CMAKE_CXX_STANDARD 26)

add_subdirectory(../LunarisEngine LunarisEngine_build)

add_executable(Game WIN32
        src/Application.cpp
        src/Application.h)

target_link_libraries(Game PRIVATE LunarisEngine)