cmake_minimum_required(VERSION 3.10)

project(OSlab1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=maybe-uninitialized")

include_directories(include)

add_executable(child src/child.cpp)
add_executable(parent src/main.cpp src/parent.cpp)
add_library(temp_file src/temp_file.cpp)

# Установка Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
  TLS_VERIFY false
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(tests tests/test_main.cpp src/parent.cpp)
target_link_libraries(tests PRIVATE temp_file gtest gtest_main)

add_test(NAME ParentChildTest COMMAND tests)