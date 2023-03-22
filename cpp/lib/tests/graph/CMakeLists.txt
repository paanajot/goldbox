add_executable(tst_lib_graph tst_graph.cpp)

target_link_libraries(
  tst_lib_graph
  PRIVATE ${CMAKE_PROJECT_NAME}::cxx20
          ${CMAKE_PROJECT_NAME}::lib_graph
          GTest::gtest
          gtest_main
)

gtest_discover_tests(tst_lib_graph)
