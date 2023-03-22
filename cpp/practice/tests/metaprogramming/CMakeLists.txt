add_executable(
  tst_metaprogramming
  tst_args_counter.cpp
  tst_crtp.cpp
  tst_concepts.cpp
  tst_print.cpp
  tst_sum.cpp
  tst_sfinae.cpp
)

target_link_libraries(
  tst_metaprogramming
  PRIVATE ${CMAKE_PROJECT_NAME}::metaprogramming
          goldboxl::cxx20
          GTest::gtest
          gtest_main
)

gtest_discover_tests(tst_metaprogramming)
