add_executable(tst_lib_unique_ptr tst_unique_ptr.cpp)

target_link_libraries(
  tst_lib_unique_ptr
  PRIVATE ${CMAKE_PROJECT_NAME}::cxx20
          ${CMAKE_PROJECT_NAME}::lib_unique_ptr
          GTest::gtest
          gtest_main
)

gtest_discover_tests(tst_lib_unique_ptr)
