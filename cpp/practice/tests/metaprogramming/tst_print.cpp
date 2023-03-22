#include <cstdint> // NOLINT
#include <functional>
#include <gtest/gtest.h>
#include <string>

#include "box/type_list.h"
#include "print.h"

namespace
{
using PrintTypeList = box::TypeList<char, std::string, uint16_t>;
using Nth0 = box::NthElement<PrintTypeList, 0>;
using Nth1 = box::NthElement<PrintTypeList, 1>;
using Nth2 = box::NthElement<PrintTypeList, 2>;

const auto print1_ptr = &print1<Nth0, Nth1, Nth2>;
const auto print2_ptr = &print2<Nth0, Nth1, Nth2>;
const auto print3_ptr = &print3<Nth0, Nth1, Nth2>;
const auto print4_ptr = &print4<Nth0, Nth1, Nth2>;
} // namespace

class PrintTest : public testing::TestWithParam<std::function<void(Nth0, Nth1, Nth2)>>
{ };

TEST_P(PrintTest, printer)
{
    testing::internal::CaptureStdout();

    constexpr uint32_t my_age = 89;
    GetParam()('I', "am", my_age);

    const auto output = testing::internal::GetCapturedStdout();

    const auto expected_result = "I am 89 \n";
    EXPECT_EQ(expected_result, output);
}

// TODO: fix test print (no idea why print is still ugly :/)
auto pretty_printer = [idx = 0](const auto&) mutable { return std::to_string(++idx); };
INSTANTIATE_TEST_SUITE_P(VariadicTemplates,
                         PrintTest,
                         testing::Values(print1_ptr, print2_ptr, print3_ptr, print4_ptr),
                         pretty_printer);
