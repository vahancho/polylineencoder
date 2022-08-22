set(TARGET polytest)

find_package(GTest CONFIG REQUIRED)

add_executable(${TARGET} main.cpp)
target_link_libraries(${TARGET} polylineencoder)
target_link_libraries(${TARGET} GTest::gtest)

add_test(NAME Test COMMAND ${TARGET})

# Copy GTest libraries to the target directory.
add_custom_command(
    TARGET ${TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:GTest::gtest>
            $<TARGET_FILE_DIR:${TARGET}>
)

# Coverage support.
option(CODE_COVERAGE "Enable coverage reporting" ON)
if (CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Add required flags (GCC & LLVM/Clang)
    target_compile_options(poly_test PUBLIC
        -O0        # no optimization
        -g         # generate debug info
        --coverage # sets all required flags
    )
    if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
        target_link_options(${TARGET} PUBLIC --coverage)
    else()
        target_link_libraries(${TARGET} PUBLIC --coverage)
    endif()
endif()
