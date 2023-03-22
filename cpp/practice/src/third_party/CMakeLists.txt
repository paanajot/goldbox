add_library(third_party STATIC json_parser.cpp converter.cpp)
add_library(third_party::third_party ALIAS third_party)

target_include_directories(third_party PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
