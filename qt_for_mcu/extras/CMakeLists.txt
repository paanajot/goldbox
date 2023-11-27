cmake_minimum_required (VERSION 3.15)

project(extras VERSION 0.0.1 LANGUAGES C CXX ASM)

if (NOT TARGET Qul::Core)
    find_package(Qul)
endif()

qul_add_target(extras main.cpp
    QML_PROJECT extras.qmlproject
)

