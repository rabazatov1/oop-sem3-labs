cmake_minimum_required(VERSION 3.20)
project(lab2 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(FATAL_ERROR "Use GNU GCC: configure with -DCMAKE_CXX_COMPILER=g++")
endif()
add_compile_options(-Wall -Wextra -Wpedantic -g -fsanitize=address -fno-omit-frame-pointer)
add_link_options(-fsanitize=address)

find_package(GTest QUIET)
if(NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
    )
    FetchContent_MakeAvailable(googletest)
endif()

add_library(lab2_lib src/matrix.cpp)
target_include_directories(lab2_lib PUBLIC include)

add_executable(lab2_main src/main.cpp)
target_link_libraries(lab2_main PRIVATE lab2_lib)

add_executable(tests test/matrix_test.cpp)
target_link_libraries(tests PRIVATE lab2_lib GTest::gtest_main)
enable_testing()
add_test(NAME Lab2Tests COMMAND tests)
