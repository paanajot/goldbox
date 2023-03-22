add_library(other INTERFACE)
add_library(${CMAKE_PROJECT_NAME}::other ALIAS other)

target_sources(
  other INTERFACE point.h small_cpp17_features.h small_cpp20_features.h
)

target_include_directories(other INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
