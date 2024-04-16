add_library(model STATIC color_detector_model.h color_detector_model.cpp)

set_target_properties(model PROPERTIES CXX_CLANG_TIDY "" CXX_CPPCHECK "")

add_library(${CMAKE_PROJECT_NAME}::model ALIAS model)

target_include_directories(
  model PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
               "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
