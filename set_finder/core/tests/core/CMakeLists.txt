add_executable(tst_core tst_set_finder.cpp)

target_link_libraries(
  tst_core
  PRIVATE ${CMAKE_PROJECT_NAME}::core
          ${CMAKE_PROJECT_NAME}::utils
          ${OpenCV_LIBS}
          goldboxl::cxx20
          GTest::gtest
          gtest_main
)

gtest_discover_tests(tst_core)
