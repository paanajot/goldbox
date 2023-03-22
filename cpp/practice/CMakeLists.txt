cmake_minimum_required(VERSION 3.14)
project(goldbox VERSION 0.1.0)

include(/usr/local/lib/cmake/goldboxl/GoldboxlTargets.cmake)
include(/usr/local/share/my_config.cmake)

add_subdirectory(src)

add_executable(main main.cpp)
if(BUILD_TESTS)
  find_package(GTest CONFIG REQUIRED)
  include(GoogleTest)
  enable_testing()

  add_subdirectory(tests)
endif()

target_link_libraries(
  main PRIVATE ${CMAKE_PROJECT_NAME}::design_patterns
               ${CMAKE_PROJECT_NAME}::other goldboxl::cxx20
)
