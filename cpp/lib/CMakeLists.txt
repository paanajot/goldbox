cmake_minimum_required(VERSION 3.14)
project(goldboxl VERSION 0.1.0)

include(cmake/cxx20.cmake)
include(cmake/my_config.cmake)

add_subdirectory(src)

if(BUILD_TESTS)
  find_package(GTest CONFIG REQUIRED)
  include(GoogleTest)
  enable_testing()

  add_subdirectory(tests)
endif()

include(cmake/install.cmake)
