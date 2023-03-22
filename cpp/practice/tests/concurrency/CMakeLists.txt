add_executable(tst_concurrency tst_atomics.cpp tst_shared_mutex.cpp)

target_link_libraries(
  tst_concurrency
  PRIVATE ${CMAKE_PROJECT_NAME}::concurrency
          goldboxl::cxx20
          GTest::gtest
          gtest_main
)

gtest_discover_tests(tst_concurrency)
