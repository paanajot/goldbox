add_library(lib_metaprogramming INTERFACE)
add_library(
  ${CMAKE_PROJECT_NAME}::lib_metaprogramming ALIAS lib_metaprogramming
)

set(header_files
    ${CMAKE_CURRENT_SOURCE_DIR}/box/type_traits.h
    ${CMAKE_CURRENT_SOURCE_DIR}/box/tuple.h
    ${CMAKE_CURRENT_SOURCE_DIR}/box/tuple_impl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/box/type_list.h
    ${CMAKE_CURRENT_SOURCE_DIR}/box/optional.h
    ${CMAKE_CURRENT_SOURCE_DIR}/box/optional_impl.h
)

target_sources(lib_metaprogramming INTERFACE ${header_files})
set_target_properties(
  lib_metaprogramming PROPERTIES PUBLIC_HEADER "${header_files}"
)

target_include_directories(
  lib_metaprogramming INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)
