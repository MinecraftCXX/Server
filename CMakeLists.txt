cmake_minimum_required(VERSION 3.19)
project(GameServer)

set(CMAKE_CXX_STANDARD 17)

include(TestBigEndian)
test_big_endian(endian)
if (endian)
    add_definitions(-DBIG_ENDIAN)
endif()

set(HEADERS include/minecraft/network/type.hpp)
set(SOURCES src/main.cpp)

add_executable(GameServer ${HEADERS} ${SOURCES})
target_include_directories(GameServer PRIVATE vendors/endian/include)
target_include_directories(GameServer PUBLIC include)