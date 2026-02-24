cmake_minimum_required(VERSION 3.10)  
project(ClientServerLab)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(PkgConfig REQUIRED)
pkg_check_modules(ZeroMQ REQUIRED libzmq)


find_package(GTest REQUIRED)
find_package(Threads REQUIRED)


include_directories(${ZeroMQ_INCLUDE_DIRS})
include_directories(${GTEST_INCLUDE_DIRS})


add_executable(client ./src/client.cpp)
target_link_libraries(client PRIVATE ${ZeroMQ_LIBRARIES} Threads::Threads)


add_executable(server ./main.cpp)
target_link_libraries(server PRIVATE ${ZeroMQ_LIBRARIES} Threads::Threads)

add_executable(tests ./tests/test.cpp)
target_link_libraries(tests PRIVATE ${ZeroMQ_LIBRARIES} GTest::gtest_main Threads::Threads)

enable_testing()
add_test(NAME tests COMMAND tests)
