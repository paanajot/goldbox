add_executable(
  tst_testing tst_message_queue_sender.cpp tst_master_converter.cpp
)

# NOTE: --wrap option can be used to wrap function calls. In C the symbols
# signature is the name that appears in the code. It is not possible to have two
# different funtions with the same name. In C++ symbol name cannot be just
# function name, because langauage allows polymorphism. That is the reason  why,
# the last --wrap option has ugly looking symbol name (it's mangled is_valid
# function name).
target_link_options(
  tst_testing
  PUBLIC
  LINKER:--wrap=mq_open,--wrap=mq_close,--wrap=mq_send,--wrap=_Z8is_validRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
)

target_link_libraries(
  tst_testing
  PRIVATE
    third_party::converter_stub # NOTE: it doesn't matter if converter (original
                                # class) is a STATIC or a SHARED library, the
                                # converter_stub has to be SHARED, unless
                                # original class is used in a header-only
                                # library. In that case converter_stub can also
                                # be an INTERFACE target.
  PRIVATE third_party::third_party_mocks # NOTE: it's important that 'static'
                                         # mock or any stubs are linked before
                                         # the implementation
  PRIVATE ${CMAKE_PROJECT_NAME}::testing
          ${CMAKE_PROJECT_NAME}::converter
          ${CMAKE_PROJECT_NAME}::testing_mocks
          goldboxl::cxx20
          GTest::gtest
          GTest::gmock
          gtest_main
)

gtest_discover_tests(tst_testing)
