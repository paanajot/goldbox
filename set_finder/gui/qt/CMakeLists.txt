cmake_minimum_required(VERSION 3.16)

project(set_finder VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Multimedia MultimediaWidgets)

qt_add_executable(set_finder main.cpp myitem.cpp)

# cmake-format: off
qt_add_qml_module(set_finder
  URI gui
  VERSION 1.0
  QML_FILES main.qml
)
# cmake-format: on

set_target_properties(
  set_finder
  PROPERTIES MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
             MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
             MACOSX_BUNDLE_SHORT_VERSION_STRING
             ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
             WIN32_EXECUTABLE TRUE
)

target_link_libraries(
  set_finder PRIVATE Qt6::Quick Qt6::Multimedia Qt6::MultimediaWidgets
)

set_target_properties(set_finder PROPERTIES
    QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android"
)

install(TARGETS set_finder BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
