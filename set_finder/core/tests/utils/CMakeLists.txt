add_executable(tst_utils tst_timer.cpp tst_thread_pool.cpp)

target_link_libraries(
  tst_utils
  PRIVATE ${CMAKE_PROJECT_NAME}::utils
          goldboxl::cxx20
          GTest::gtest
          gtest_main
)

gtest_discover_tests(tst_utils)
