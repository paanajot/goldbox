project(set_finder_gui)
cmake_minimum_required(VERSION 3.18)

# opencv for android
# TODO: remove if not needed
set(OpenCV_STATIC ON)
set(OpenCV_DIR $ENV{OPENCV_ANDROID}/sdk/native/jni)
find_package(OpenCV REQUIRED)

# set finder core
include(/workdir/set_finder_core/${ANDROID_ABI}/cmake/SetFinderTargets.cmake)

set(CMAKE_VERBOSE_MAKEFILE on)
set(COMMON_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../common)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Werror")

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")
add_library(camera_textureview SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/android_main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/camera_manager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/camera_engine.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/preview_processor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/camera_listeners.cpp
        ${COMMON_SOURCE_DIR}/utils/camera_utils.cpp)

target_include_directories(camera_textureview PRIVATE /workdir/libyuv/${ANDROID_ABI}/include ${COMMON_SOURCE_DIR})

# TODO: why we need to link to detector ?
target_link_libraries(camera_textureview  dl android log m camera2ndk mediandk nativewindow /workdir/libyuv/${ANDROID_ABI}/lib/libyuv.so
                                          set_finder::core)
