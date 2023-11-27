# install core
include(GNUInstallDirs)

# cmake-format: off
set(SETFINDER_CMAKE_INSTALL_PATH    "${CMAKE_INSTALL_PREFIX}/cmake")
set(SETFINDER_LIB_INSTALL_PATH      "${CMAKE_INSTALL_PREFIX}/lib")
set(SETFINDER_INCLUDE_INSTALL_PATH  "${CMAKE_INSTALL_PREFIX}/include")
# cmake-format: on

set(header_files ${CMAKE_SOURCE_DIR}/src/core/set_processor.h
                 ${CMAKE_SOURCE_DIR}/src/utils/logger.h
)
set_target_properties(core PROPERTIES PUBLIC_HEADER "${header_files}")

install(
  TARGETS core
  EXPORT SetFinderCore
  DESTINATION ${SETFINDER_LIB_INSTALL_PATH}
  PUBLIC_HEADER DESTINATION ${SETFINDER_INCLUDE_INSTALL_PATH}/core
  INCLUDES
  DESTINATION ${SETFINDER_INCLUDE_INSTALL_PATH}
)

install(EXPORT SetFinderCore DESTINATION ${SETFINDER_CMAKE_INSTALL_PATH}
        NAMESPACE ${CMAKE_PROJECT_NAME}::
)

install(FILES cmake/SetFinderCoreConfig.cmake
        DESTINATION ${SETFINDER_CMAKE_INSTALL_PATH}
)
