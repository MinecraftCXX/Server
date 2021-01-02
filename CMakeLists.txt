cmake_minimum_required(VERSION 3.19)
project(GameServer)

set(CMAKE_CXX_STANDARD 17)

include(TestBigEndian)
test_big_endian(endian)
if (endian)
    add_definitions(-DBIG_ENDIAN)
endif()

set(HEADERS include/minecraft/network/type.hpp)
set(SOURCES src/network/_placeholder.cpp)
set(ENTRY src/main.cpp)

add_executable(GameServer ${HEADERS} ${SOURCES} ${ENTRY})
add_library(libGameServer STATIC ${HEADERS} ${SOURCES})

target_include_directories(GameServer PUBLIC include)
target_include_directories(libGameServer PUBLIC include)

file(GLOB FILES "${CMAKE_CURRENT_SOURCE_DIR}/vendors/gtest")
list(LENGTH FILES DIR_CONTENT)

if (NOT DIR_CONTENT EQUAL 0)
    add_subdirectory(vendors/gtest)
    add_subdirectory(tests)
else()
    find_package(GTest)
    add_subdirectory(tests)
endif()