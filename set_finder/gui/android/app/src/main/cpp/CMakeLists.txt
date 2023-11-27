project(set_finder_gui)
cmake_minimum_required(VERSION 3.18)

# opencv for android TODO: remove if not needed
set(OpenCV_STATIC ON)
set(OpenCV_DIR $ENV{OPENCV_ANDROID}/sdk/native/jni)
find_package(OpenCV REQUIRED)

# set finder core
set(SetFinderCore_DIR $ENV{SET_FINDER_CORE}/${ANDROID_ABI}/cmake/)
find_package(SetFinderCore REQUIRED)

# libyuv
add_library(yuv_converter SHARED IMPORTED)
set_target_properties(
  yuv_converter
  PROPERTIES IMPORTED_LOCATION
             "$ENV{YUV_CONVERTER}/${ANDROID_ABI}/lib/libyuv.so"
             INTERFACE_INCLUDE_DIRECTORIES
             "$ENV{YUV_CONVERTER}/${ANDROID_ABI}/include"
)

# set finder gui
set(CMAKE_VERBOSE_MAKEFILE on)
set(COMMON_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../common)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Werror")

set(CMAKE_SHARED_LINKER_FLAGS
    "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate"
)
add_library(
  camera_textureview SHARED
  ${CMAKE_CURRENT_SOURCE_DIR}/android_main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/camera_manager.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/camera_engine.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/preview_processor.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/camera_listeners.cpp
  ${COMMON_SOURCE_DIR}/utils/camera_utils.cpp
)

target_include_directories(camera_textureview PRIVATE ${COMMON_SOURCE_DIR})
target_link_libraries(
  camera_textureview
  dl
  android
  log
  m
  camera2ndk
  mediandk
  nativewindow
  set_finder::core
  yuv_converter
)
