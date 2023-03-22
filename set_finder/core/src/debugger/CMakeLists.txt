add_library(debugger STATIC set_debugger.cpp image_writer.cpp)
add_library(${CMAKE_PROJECT_NAME}::debugger ALIAS debugger)

target_link_libraries(
  debugger
  PRIVATE ${OpenCV_LIBS}
          ${CMAKE_PROJECT_NAME}::utils
          ${CMAKE_PROJECT_NAME}::detector
          ${CMAKE_PROJECT_NAME}::card_extractor
          goldboxl::cxx20
)
