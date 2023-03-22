add_executable(tst_ranges tst_examples.cpp)

target_link_libraries(
  tst_ranges
  PRIVATE ${CMAKE_PROJECT_NAME}::ranges
          goldboxl::cxx20
          GTest::gtest
          gtest_main
)

gtest_discover_tests(tst_ranges)
