cmake_minimum_required(VERSION 3.5)

project(engine_shared LANGUAGES CXX)
find_package(SDL3 REQUIRED)
add_library(engine_shared SHARED engine.cpp engine.h)
set_target_properties(engine_shared PROPERTIES PREFIX "")
target_link_libraries(engine_shared SDL3::SDL3)
target_compile_features(engine_shared PUBLIC cxx_std_20)

add_executable(SharedSDL game.cpp)
target_link_libraries(SharedSDL engine_shared)
target_compile_features(SharedSDL PUBLIC cxx_std_20)
