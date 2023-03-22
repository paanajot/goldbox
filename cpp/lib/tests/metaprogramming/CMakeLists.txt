add_executable(
  tst_lib_metaprogramming
  tst_tuple.cpp
  tst_type_list.cpp
  tst_type_traits.cpp
  tst_optional.cpp
)

target_link_libraries(
  tst_lib_metaprogramming
  PRIVATE ${CMAKE_PROJECT_NAME}::cxx20
          ${CMAKE_PROJECT_NAME}::lib_metaprogramming
          GTest::gtest
          gtest_main
)

gtest_discover_tests(tst_lib_metaprogramming)
