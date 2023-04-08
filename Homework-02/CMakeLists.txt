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
    add_executable(my_sdl_static main.cpp)
    target_compile_features(my_sdl_static PUBLIC cxx_std_20)  
    target_link_libraries(my_sdl_static PRIVATE SDL3::SDL3-static)
    add_test(NAME check-exit-code-SDL-STATIC COMMAND my_sdl_static) 
endif()

if(SHARED OR BOTH)
    add_executable(my_sdl_shared main.cpp)
    target_compile_features(my_sdl_shared PUBLIC cxx_std_20)    
    target_link_libraries(my_sdl_shared PRIVATE SDL3::SDL3-shared)
    add_test(NAME check-exit-code-SDL-SHARED COMMAND my_sdl_shared)
endif()




