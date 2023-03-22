add_library(testing_mocks STATIC message_queue_mock.cpp)
add_library(${CMAKE_PROJECT_NAME}::testing_mocks ALIAS testing_mocks)

target_link_libraries(testing_mocks PUBLIC GTest::gmock)

target_include_directories(testing_mocks PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
