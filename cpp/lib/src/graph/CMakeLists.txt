add_library(lib_graph INTERFACE)
add_library(${CMAKE_PROJECT_NAME}::lib_graph ALIAS lib_graph)

set(header_files
    ${CMAKE_CURRENT_SOURCE_DIR}/box/algorithm_details.h
    ${CMAKE_CURRENT_SOURCE_DIR}/box/algorithm.h
    ${CMAKE_CURRENT_SOURCE_DIR}/box/edge.h
    ${CMAKE_CURRENT_SOURCE_DIR}/box/graph.h
    ${CMAKE_CURRENT_SOURCE_DIR}/box/list.h
    ${CMAKE_CURRENT_SOURCE_DIR}/box/node.h
)
target_sources(lib_graph INTERFACE ${header_files})

target_include_directories(lib_graph INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
