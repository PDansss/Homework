cmake_minimum_required(VERSION 3.15)
project(05-2-my_opengl_triangles CXX C)

add_library(engine_lib SHARED engine.cpp engine.hpp glad/glad.c glad/glad.h KHR/khrplatform.h)
set_target_properties(engine_lib PROPERTIES PREFIX "")
find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)
target_link_libraries(engine_lib SDL3::SDL3-shared)
target_include_directories(engine_lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(engine_lib PUBLIC cxx_std_20)

add_executable(my_game game.cpp)
target_link_libraries(my_game PUBLIC engine_library)
target_compile_features(my_game PUBLIC cxx_std_20)
