cmake_minimum_required(VERSION 3.15)
project(06-1-vertex-morfing CXX C)

add_library(engine_library SHARED engine.cpp engine.hpp picopng.hpp glad/glad.c glad/glad.h KHR/khrplatform.h)
set_target_properties(engine_library PROPERTIES PREFIX "")
find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)
target_link_libraries(engine_library SDL3::SDL3-shared)
target_include_directories(engine_library PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(engine_library PUBLIC cxx_std_20)

add_executable(my_game game.cpp)
target_link_libraries(my_game PUBLIC engine_library)
target_compile_features(my_game PUBLIC cxx_std_20)
