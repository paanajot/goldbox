# message queue sender
add_library(testing INTERFACE)
add_library(${CMAKE_PROJECT_NAME}::testing ALIAS testing)

target_sources(testing INTERFACE message_queue_sender.h)
target_link_libraries(testing INTERFACE third_party::third_party rt)

target_include_directories(testing INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# converter
add_library(converter STATIC master_converter.cpp)
add_library(${CMAKE_PROJECT_NAME}::converter ALIAS converter)

target_link_libraries(converter PRIVATE third_party::third_party)

target_include_directories(converter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
