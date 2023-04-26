cmake_minimum_required(VERSION 3.16..3.22)
project(04-0-render-basic)

add_executable(
  canvas
  src/00_canvas_main.cpp
  src/00_canvas.cpp
  include/00_canvas.h
)

target_include_directories(canvas PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

add_executable(
  line
  src/01_line_main.cpp
  src/00_canvas.cpp
  src/01_line.cpp
  include/01_line.h
)

target_include_directories(line PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

add_executable(
  triangle
  src/00_canvas.cpp
  src/01_line.cpp
  src/02_triangle.cpp
  src/02_triangle_main.cpp
  include/02_triangle.h
)

target_include_directories(triangle PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

target_compile_features(canvas PUBLIC cxx_std_20)
target_compile_features(line PUBLIC cxx_std_20)
target_compile_features(triangle PUBLIC cxx_std_20)
