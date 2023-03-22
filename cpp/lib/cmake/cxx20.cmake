cmake_minimum_required(VERSION 3.14)

add_library(cxx20 INTERFACE)
add_library(${CMAKE_PROJECT_NAME}::cxx20 ALIAS cxx20)

target_compile_options(cxx20 INTERFACE -Wall -Wextra -Wpedantic)

target_compile_features(cxx20 INTERFACE cxx_std_20)
