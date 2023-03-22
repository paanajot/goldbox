add_library(design_patterns STATIC)
add_library(${CMAKE_PROJECT_NAME}::design_patterns ALIAS design_patterns)

add_subdirectory(singleton)
add_subdirectory(adapter)
add_subdirectory(good_practices)
add_subdirectory(strategy)

target_include_directories(
  design_patterns INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)
