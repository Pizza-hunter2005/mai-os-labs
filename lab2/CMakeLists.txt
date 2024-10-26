cmake_minimum_required(VERSION 3.10)

project(OSlab2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=maybe-uninitialized")

set(SOURCES src/Hex512.cpp src/process.cpp src/temp_file.cpp)
include_directories(include)
add_executable(main main.cpp ${SOURCES})

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

# Линковка потоков (pthread) для многопоточной работы
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(main Threads::Threads)

add_executable(tests tests/test.cpp ${SOURCES})
target_link_libraries(tests ${GTEST_LIBRARIES} Threads::Threads)
add_test(NAME tests COMMAND tests)
