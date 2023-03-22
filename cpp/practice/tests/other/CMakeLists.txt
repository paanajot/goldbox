add_executable(tst_other tst_tmp.cpp)

target_link_libraries(
  tst_other
  PRIVATE ${CMAKE_PROJECT_NAME}::other
          goldboxl::cxx20
          GTest::gtest
          gtest_main
)

gtest_discover_tests(tst_other)
