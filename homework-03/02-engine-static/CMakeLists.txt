cmake_minimum_required(VERSION 3.5)

project(engine_static LANGUAGES CXX)
find_package(SDL3 REQUIRED)

add_library(engine_static STATIC engine.cpp engine.h)
set_target_properties(engine_static PROPERTIES PREFIX "")
target_link_libraries(engine_static SDL3::SDL3)
target_compile_features(engine_static PUBLIC cxx_std_20)

add_executable(StaticSDL main.cpp)
target_link_libraries(StaticSDL engine_static)
target_compile_features(StaticSDL PUBLIC cxx_std_20)
