add_library(
  core SHARED
  engine_supervisor.cpp
  set_finder.cpp
  set_generator.cpp
  set_engine.cpp
  set_processor.cpp
)
add_library(${CMAKE_PROJECT_NAME}::core ALIAS core)

target_link_libraries(
  core
  PRIVATE ${OpenCV_LIBS}
          ${CMAKE_PROJECT_NAME}::card_extractor
          ${CMAKE_PROJECT_NAME}::debugger
          ${CMAKE_PROJECT_NAME}::detector
          ${CMAKE_PROJECT_NAME}::utils
          goldboxl::cxx20
)
# TODO: move it
target_compile_options(core PRIVATE "-O2")

target_include_directories(
  core PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
              "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

set(header_files ${CMAKE_CURRENT_SOURCE_DIR}/set_processor.h
                 ${CMAKE_SOURCE_DIR}/src/utils/logger.h
)
set_target_properties(core PROPERTIES PUBLIC_HEADER "${header_files}")
