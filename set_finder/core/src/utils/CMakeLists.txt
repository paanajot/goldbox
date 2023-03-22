add_library(utils STATIC logger.cpp algorithm.cpp)

add_library(${CMAKE_PROJECT_NAME}::utils ALIAS utils)

target_link_libraries(utils PRIVATE ${OpenCV_LIBS} goldboxl::cxx20)

target_include_directories(
  utils PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
               "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
