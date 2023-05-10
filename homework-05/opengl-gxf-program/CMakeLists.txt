cmake_minimum_required(VERSION 3.5)
project(gfx-program-with-hot-reload)

add_executable(gfx-program engine.cpp engine.h glad/glad.c glad/glad.h KHR/khrplatform.h)
target_compile_features(gfx-program PUBLIC cxx_std_20)

set_target_properties(gfx-program PROPERTIES ENABLE_EXPORTS 1)

find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)
target_link_libraries(gfx-program PRIVATE SDL3::SDL3-shared)
target_include_directories(gfx-program PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(gfx-program PUBLIC cxx_std_20)

add_library(Game SHARED game.cpp)
target_compile_features(Game PUBLIC cxx_std_20)
target_link_libraries(Game PRIVATE gfx-program)
