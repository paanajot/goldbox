add_executable(tst_detector tst_detector.cpp)

target_link_libraries(
  tst_detector
  PRIVATE ${CMAKE_PROJECT_NAME}::detector
          ${CMAKE_PROJECT_NAME}::utils
          ${OpenCV_LIBS}
          goldboxl::cxx20
          GTest::gtest
          gtest_main
)

gtest_discover_tests(tst_detector)
