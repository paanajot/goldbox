add_library(metaprogramming INTERFACE)
add_library(${CMAKE_PROJECT_NAME}::metaprogramming ALIAS metaprogramming)

target_sources(
  metaprogramming
  INTERFACE args_counter.h
            sum.h
            print.h
            concepts.h
            sfinae.h
)

target_include_directories(
  metaprogramming INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)
