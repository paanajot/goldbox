add_library(ranges INTERFACE)
add_library(${CMAKE_PROJECT_NAME}::ranges ALIAS ranges)

target_sources(ranges INTERFACE examples.h)

target_include_directories(ranges INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
