cmake_minimum_required(VERSION 3.5)
project(Homework1)

file(COPY ${CMAKE_SOURCE_DIR}/readonly DESTINATION ${CMAKE_CURRENT_BINARY_DIR} FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
#file(COPY ${CMAKE_SOURCE_DIR}/readonly DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(Homework-01)

enable_testing()

add_test(NAME TestStatic COMMAND StaticMain)
add_test(NAME TestShared COMMAND SharedMain)


add_test(NAME NegativeTestStatic COMMAND sh -c "${CMAKE_CURRENT_BINARY_DIR}/Homework-01/StaticMain >> readonly")
set_tests_properties(NegativeTestStatic PROPERTIES WILL_FAIL TRUE)

add_test(NAME NegativeTestShared COMMAND sh -c "${CMAKE_CURRENT_BINARY_DIR}/Homework-01/SharedMain >> readonly")
set_tests_properties(NegativeTestShared PROPERTIES WILL_FAIL TRUE)

