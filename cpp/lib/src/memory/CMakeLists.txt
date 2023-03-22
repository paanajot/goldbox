add_library(lib_unique_ptr INTERFACE)
add_library(${CMAKE_PROJECT_NAME}::lib_unique_ptr ALIAS lib_unique_ptr)

set(header_files ${CMAKE_CURRENT_SOURCE_DIR}/box/unique_ptr.h
                 ${CMAKE_CURRENT_SOURCE_DIR}/box/unique_ptr_impl.h
)

target_sources(lib_unique_ptr INTERFACE ${header_files})
set_target_properties(lib_unique_ptr PROPERTIES PUBLIC_HEADER "${header_files}")

target_include_directories(lib_unique_ptr INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
