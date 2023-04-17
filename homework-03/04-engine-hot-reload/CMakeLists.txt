cmake_minimum_required(VERSION 3.5)
project(03-4-sdl-loop-hot-reload)

add_executable(Engine-hot-reload engine.cpp engine.h)
target_compile_features(Engine-hot-reload PUBLIC cxx_std_20)

set_target_properties(Engine-hot-reload PROPERTIES ENABLE_EXPORTS 1)

find_package(SDL3 REQUIRED)

target_link_libraries(Engine-hot-reload PRIVATE SDL3::SDL3-shared)
add_library(Game SHARED game.cpp)

target_compile_features(Game PUBLIC cxx_std_20)
target_link_libraries(Game PRIVATE Engine-hot-reload)
