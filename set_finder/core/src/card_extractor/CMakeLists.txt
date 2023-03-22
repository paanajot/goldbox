add_library(
  card_extractor STATIC
  image_filter.cpp
  contours_extractor.cpp
  filter_optimizer.cpp
  extractor.cpp
  corners_helper.cpp
)

add_library(${CMAKE_PROJECT_NAME}::card_extractor ALIAS card_extractor)

target_link_libraries(
  card_extractor PUBLIC ${OpenCV_LIBS} PRIVATE ${CMAKE_PROJECT_NAME}::utils
                                               goldboxl::cxx20
)

target_include_directories(
  card_extractor PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
                        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
