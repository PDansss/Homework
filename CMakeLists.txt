cmake_minimum_required(VERSION 3.5)
project(Homework1)

add_subdirectory(homework-01)
add_subdirectory(homework-02)
add_subdirectory(homework-03)

enable_testing()

add_test(NAME Test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/homework-01/hello/Main)
add_test(NAME TestStatic COMMAND ${CMAKE_CURRENT_BINARY_DIR}/homework-01/hello-with-library/StaticMain)
add_test(NAME TestShared COMMAND ${CMAKE_CURRENT_BINARY_DIR}/homework-01/hello-with-library/SharedMain)

