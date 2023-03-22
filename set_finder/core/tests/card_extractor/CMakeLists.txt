add_executable(tst_card_extractor tst_card_extractor.cpp)

target_link_libraries(
  tst_card_extractor
  PRIVATE ${CMAKE_PROJECT_NAME}::card_extractor
          goldboxl::cxx20
          GTest::gtest
          gtest_main
)

gtest_discover_tests(tst_card_extractor)
