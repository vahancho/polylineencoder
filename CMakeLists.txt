cmake_minimum_required(VERSION 3.12)

project(polylineencoder)
set (CMAKE_CXX_STANDARD 11)

add_library(encoder
  src/polylineencoder.cpp
)

add_executable(poly_test
  test/main.cpp
)

target_link_libraries(poly_test
  PRIVATE encoder
)

enable_testing()
add_test(NAME Test COMMAND poly_test)
