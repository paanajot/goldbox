cmake_minimum_required(VERSION 3.14)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)

option(BUILD_TESTS "build tests" OFF)
option(RUN_IWYU "run include-what-you-use" ON)
option(RUN_CLANG_TIDY "run clang tidy" ON)
option(RUN_CPPCHECK "run cppcheck" ON)
option(RUN_ADDRESS_SANITIZER "run address sanitizer" ON)
option(DISABLE_TOOLS "disable all tools" OFF)

if(DISABLE_TOOLS)
  set(RUN_CLANG_TIDY OFF CACHE BOOL "" FORCE)
  set(RUN_CPPCHECK OFF CACHE BOOL "" FORCE)
  set(RUN_ADDRESS_SANITIZER OFF CACHE BOOL "" FORCE)
  set(RUN_IWYU OFF CACHE BOOL "" FORCE)
endif()

if(${CMAKE_PROJECT_NAME} STREQUAL "goldboxl")
  set(IWYU_CONIG_PATH ${CMAKE_SOURCE_DIR}/config/iwyu_map_file.imp)
else()
  set(IWYU_CONIG_PATH /usr/local/share/iwyu_map_file.imp)
endif()

if(RUN_IWYU)
  set(iwyu_path /iwyu/build/bin/include-what-you-use)

  set(iwyu_path_and_options ${iwyu_path} -Xiwyu
                            --mapping_file=${IWYU_CONIG_PATH}
  )

  set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path_and_options})
endif()

if(RUN_CLANG_TIDY)
  set(CMAKE_CXX_CLANG_TIDY clang-tidy-14; -header-filter=.;
                           -format-style=file;-warnings-as-errors=*;
  )
endif()

if(RUN_CPPCHECK)
  set(CMAKE_CXX_CPPCHECK cppcheck; --inline-suppr)
endif()

if(RUN_ADDRESS_SANITIZER)
  # NOTE: AddressSanitizer is a memory error detector for C/C++.
  # https://github.com/google/sanitizers/wiki/AddressSanitizer
  add_compile_options(-fsanitize=address)
  add_link_options(-fsanitize=address)

  # To get nicer stack traces in error messages
  add_compile_options(-fno-omit-frame-pointer)
  add_link_options(-fno-omit-frame-pointer)
endif()

# clang-tidy-diff
macro(add_clang_tidy_diff_target)
  find_package(Clang QUIET)
  if(CLANG_INSTALL_PREFIX)
    file(GLOB_RECURSE clang-tidy-diff-script
         ${CLANG_INSTALL_PREFIX}/*clang-tidy-diff.py
    )
    if(clang-tidy-diff-script AND CMAKE_EXPORT_COMPILE_COMMANDS)
      if(${CMAKE_EXPORT_COMPILE_COMMANDS})
        add_custom_target(
          clang-tidy-diff
          COMMAND
            cd ${CMAKE_SOURCE_DIR} && git diff origin/master... -- 'util/*'
            'include/*' 'src/*' | ${clang-tidy-diff-script} -p1 -path
            ${CMAKE_BINARY_DIR} -use-color -j5 -quiet
        )
      endif()
    endif()
  endif()
endmacro()
