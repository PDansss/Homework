cmake_minimum_required(VERSION 3.20)
project(my_sdl)

option(BOTH "Link Both" ON)
option(STATIC "Link Static" OFF)
option(SHARED "Link Shared" OFF)

find_package(SDL3 REQUIRED)

enable_testing()

if(STATIC)
    set(SHARED OFF)
    set(BOTH OFF)
endif() 

if(SHARED)
    set(STATIC OFF)
    set(BOTH OFF)
endif() 
  
if(STATIC OR BOTH)
    add_executable(my_sdl_static src/main.cpp)
    target_compile_features(my_sdl_static PUBLIC cxx_std_20)  
    
    get_target_property(PATH_TO_SDL3_STATIC SDL3::SDL3-static LOCATION)
    message(STATUS "path to static libSDL3.a [${PATH_TO_SDL3_STATIC}]")
    
    target_link_libraries(my_sdl_static PRIVATE SDL3::SDL3-static)
    add_test(NAME check-exit-code-SDL-STATIC COMMAND my_sdl_static) 
endif()

if(SHARED OR BOTH)
    add_executable(my_sdl_shared src/main.cpp)
    
    get_target_property(PATH_TO_SDL3_SHARED SDL3::SDL3 LOCATION)
    message(STATUS "path to static libSDL3.so [${PATH_TO_SDL3_SHARED}]")
    target_compile_features(my_sdl_shared PUBLIC cxx_std_20)    
    
    target_link_libraries(my_sdl_shared PRIVATE SDL3::SDL3)
    add_test(NAME check-exit-code-SDL-SHARED COMMAND my_sdl_shared)
endif()
