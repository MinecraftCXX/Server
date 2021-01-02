set(TESTS network_type.cpp)

set(BINARY ${CMAKE_PROJECT_NAME}_Tests)

add_executable(${BINARY} ${TESTS})

enable_testing()
add_test(NAME ${BINARY} COMMAND ${BINARY})

if (GTest_Found)
    target_link_libraries(${BINARY} PUBLIC libGameServer GTest::Main)
else()
    target_link_libraries(${BINARY} PUBLIC libGameServer gtest_main)
endif()