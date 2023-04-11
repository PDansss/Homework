cmake_minimum_required(VERSION 3.20)
project(testdynapi)
find_package(SDL2 REQUIRED)

add_executable(SDL_STATIC src/main.cpp)
target_link_libraries(SDL_STATIC PRIVATE SDL2::SDL2-static)

add_executable(SDL_SHARED src/main.cpp)
target_link_libraries(SDL_SHARED SDL2::SDL2)
