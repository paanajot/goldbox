add_library(
  detector STATIC
  card_preprocessor.cpp
  number_detector.cpp
  shape_detector.cpp
  shading_detector.cpp
  color_detector.cpp
)

add_library(${CMAKE_PROJECT_NAME}::detector ALIAS detector)

target_link_libraries(
  detector
  PRIVATE ${OpenCV_LIBS}
          ${CMAKE_PROJECT_NAME}::model
          ${CMAKE_PROJECT_NAME}::utils
          goldboxl::cxx20
)

target_include_directories(
  detector PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
                  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
  PRIVATE "/usr/local/include/" # TODO: try to find a better way (fdeep)
)
