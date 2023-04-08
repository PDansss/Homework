cmake_minimum_required(VERSION 3.20)
project(my_sdl)

option(SDL-STATIC "Link Static" ON)

add_executable(my_sdl src/main.cpp)
target_compile_features(my_sdl PUBLIC cxx_std_20)

find_package(SDL3 REQUIRED)

enable_testing()

if(SDL-STATIC)
    target_link_libraries(my_sdl PRIVATE SDL3::SDL3-static)
    add_test(NAME check-exit-code-SDL-STATIC COMMAND my_sdl)
else()
    target_link_libraries(my_sdl PRIVATE SDL3::SDL3-shared)
    add_test(NAME check-exit-code-SDL-SHARED COMMAND my_sdl)
endif()




