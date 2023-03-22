cmake_minimum_required(VERSION 3.14)

project(external_project_example VERSION 0.1.0)

set(installDir ${CMAKE_CURRENT_BINARY_DIR}/core)

include(ExternalProject)
ExternalProject_Add(
  set_finder_core
  SOURCE_DIR /repo/set_finder/core
  INSTALL_DIR ${installDir}
  BUILD_BYPRODUCTS <INSTALL_DIR>/lib/libcore.so ${installDir}/include/
  BUILD_ALWAYS TRUE
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
)

add_library(set_finder::core SHARED IMPORTED)

# NOTE: the problem with that approach is that include folder doesn't exists
# before the compilation, therefore the cmake configuration step fails. Another
# drawback is that the dependencies carried by the target are not preserved.
set_target_properties(
  set_finder::core
  PROPERTIES IMPORTED_LOCATION
             ${installDir}/lib/libcore.so # INTERFACE_INCLUDE_DIRECTORIES
             # ${installDir}/include/
)

add_dependencies(set_finder::core set_finder_core)

add_executable(main main.cpp)
target_link_libraries(main set_finder::core)
