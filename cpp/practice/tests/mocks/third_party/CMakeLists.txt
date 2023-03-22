# third party mocks
add_library(third_party_mocks INTERFACE)
add_library(third_party::third_party_mocks ALIAS third_party_mocks)

target_sources(
  third_party_mocks INTERFACE json_parser_mock.h json_parser.h converter.h
)

target_link_libraries(third_party_mocks INTERFACE GTest::gmock)

target_include_directories(
  third_party_mocks INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

# converter stub
add_library(converter_stub SHARED converter.cpp)
add_library(third_party::converter_stub ALIAS converter_stub)

target_include_directories(converter_stub PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
