cmake_minimum_required(VERSION 3.5)

project(simple_loop LANGUAGES CXX)
find_package(SDL3 REQUIRED)

add_executable(simple_loop main.cpp)
target_link_libraries(simple_loop SDL3::SDL3)
target_compile_features(simple_loop PUBLIC cxx_std_20)
