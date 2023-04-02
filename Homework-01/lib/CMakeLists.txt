cmake_minimum_required(VERSION 3.5)

project(library LANGUAGES CXX)

add_library(Static STATIC include/lib.h src/lib.cpp)
set_target_properties(Static PROPERTIES PREFIX "")
target_include_directories(Static PUBLIC include)

add_library(Shared SHARED include/lib.h src/lib.cpp)
set_target_properties(Shared PROPERTIES PREFIX "")
target_include_directories(Shared PUBLIC include)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


