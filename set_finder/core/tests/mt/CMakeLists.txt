add_executable(tst_features tst_features.cpp)

target_link_libraries(
  tst_features
  PRIVATE ${CMAKE_PROJECT_NAME}::core
          ${CMAKE_PROJECT_NAME}::card_extractor
          ${CMAKE_PROJECT_NAME}::detector
          goldboxl::cxx20
          GTest::gtest
          gtest_main
)

gtest_discover_tests(tst_features)
