cmake_minimum_required(VERSION 3.5)

add_subdirectory(lib)

add_executable(StaticMain src/main-hello.cpp)
target_link_libraries(StaticMain PUBLIC Static)

add_executable(SharedMain src/main-hello.cpp)
target_link_libraries(SharedMain PUBLIC Shared)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
