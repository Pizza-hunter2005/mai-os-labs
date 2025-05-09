cmake_minimum_required(VERSION 3.10)

project(DynamicLibExample)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
set(BUILD_DIR "${PROJECT_SOURCE_DIR}/build")

add_library(PrimeLibrary SHARED ${SOURCE_DIR}/PrimeCount.cpp)
target_include_directories(PrimeLibrary PUBLIC ${SOURCE_DIR})

add_library(SquareLibrary SHARED ${SOURCE_DIR}/Square.cpp)
target_include_directories(SquareLibrary PUBLIC ${SOURCE_DIR})

add_executable(main_static ${SOURCE_DIR}/main_static.cpp)
add_executable(main_dynamic ${SOURCE_DIR}/main_dynamic.cpp)

target_link_libraries(main_static PRIVATE PrimeLibrary SquareLibrary)
set_target_properties(main_static main_dynamic PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})

add_custom_command(TARGET main_dynamic POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:PrimeLibrary> ${BUILD_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SquareLibrary> ${BUILD_DIR})

    enable_testing()

# Подключение Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(tests tests/tests.cpp)

target_link_libraries(tests PRIVATE GTest::GTest GTest::Main PrimeLibrary SquareLibrary)

add_test(NAME RunTests COMMAND tests)