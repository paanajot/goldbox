cmake_minimum_required(VERSION 3.14)

set(RUN_IWYU OFF CACHE BOOL "IWYU tool doesn't work good with the opencv" FORCE)

project(set_finder VERSION 0.1.0)

include(/usr/local/lib/cmake/goldboxl/GoldboxlTargets.cmake)
include(/usr/local/share/my_config.cmake)

# TODO: understand why
add_compile_options(-fPIC)

# opencv package
set(OpenCV_STATIC ON)
find_package(OpenCV REQUIRED)

find_package(frugally_deep REQUIRED)

add_subdirectory(src)

add_executable(main main.cpp)
add_executable(contours_drawer contours_drawer.cpp)

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(
  main PRIVATE ${OpenCV_LIBS} ${CMAKE_PROJECT_NAME}::core goldboxl::cxx20
)

target_link_libraries(
  contours_drawer PRIVATE ${OpenCV_LIBS} ${CMAKE_PROJECT_NAME}::core
                          goldboxl::cxx20
)

if(BUILD_TESTS)
  find_package(GTest CONFIG REQUIRED)
  include(GoogleTest)
  enable_testing()

  add_subdirectory(tests)
endif()

include(cmake/install.cmake)
