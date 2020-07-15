cmake_minimum_required(VERSION 3.9)

project(polylineencoder)
set (CMAKE_CXX_STANDARD 11)

add_library(encoder
  src/polylineencoder.h
  src/polylineencoder.cpp
)

add_executable(poly_test
  test/main.cpp
)

target_link_libraries(poly_test
  PRIVATE encoder
)

# Includes
target_include_directories(poly_test PRIVATE src)

enable_testing()

find_package(GTest CONFIG REQUIRED)
add_test(NAME Test COMMAND poly_test GTest::gtest)

# Copy GTest libraries to the target directory.
add_custom_command(
    TARGET poly_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:GTest::gtest>
        $<TARGET_FILE_DIR:poly_test>
)

# Coverage support.
option(CODE_COVERAGE "Enable coverage reporting" ON)
if (CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  # Add required flags (GCC & LLVM/Clang)
  target_compile_options(encoder INTERFACE
    -O0        # no optimization
    -g         # generate debug info
    --coverage # sets all required flags
  )
  if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
    target_link_options(encoder INTERFACE --coverage)
  else()
    target_link_libraries(encoder INTERFACE --coverage)
  endif()
endif()
