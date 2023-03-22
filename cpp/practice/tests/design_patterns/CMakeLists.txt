add_executable(
  tst_design_patterns
  tst_singleton.cpp
  tst_adapter.cpp
  tst_facade.cpp
  tst_strategy.cpp
)

target_link_libraries(
  tst_design_patterns
  PRIVATE ${CMAKE_PROJECT_NAME}::design_patterns
          goldboxl::cxx20
          GTest::gtest
          gtest_main
)

gtest_discover_tests(tst_design_patterns)
