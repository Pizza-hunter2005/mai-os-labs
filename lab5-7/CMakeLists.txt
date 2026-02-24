cmake_minimum_required(VERSION 3.0)
project(ClientServerLab)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ZeroMQ REQUIRED libzmq)

set(SOURCES_SERVER
    src/Server.cpp
    include/CalculationNode.h
    include/BalancedTree.h
    include/ZMQFunctions.h
)

set(SOURCES_CLIENT
    src/Client.cpp
    include/CalculationNode.h
    include/ZMQFunctions.h
)

add_executable(server ${SOURCES_SERVER})
add_executable(client ${SOURCES_CLIENT})


target_link_libraries(server PRIVATE ${ZeroMQ_LIBRARIES})
target_link_libraries(client PRIVATE ${ZeroMQ_LIBRARIES})


find_package(GTest REQUIRED)

enable_testing()
add_executable(tests tests/test.cpp)
target_link_libraries(tests PRIVATE ${ZeroMQ_LIBRARIES} GTest::GTest GTest::Main)

add_test(NAME AllTests COMMAND tests)

