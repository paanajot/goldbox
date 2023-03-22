#include <gtest/gtest.h>

#include "sfinae.h"

namespace
{
constexpr float float_value = 2.1F;
}

TEST(sfinae, print_1_float)
{
    testing::internal::CaptureStdout();

    print_1(float_value);

    const auto output = testing::internal::GetCapturedStdout();
    const auto expected_result = "Floating point: 2.1\n";
    EXPECT_EQ(expected_result, output);
}

TEST(sfinae, print_1_int)
{
    constexpr int int_value = 21;
    testing::internal::CaptureStdout();

    print_1(int_value);

    const auto output = testing::internal::GetCapturedStdout();
    const auto expected_result = "Integral: 21\n";
    EXPECT_EQ(expected_result, output);
}

TEST(sfinae, print_2)
{
    testing::internal::CaptureStdout();

    print_2(float_value);

    const auto output = testing::internal::GetCapturedStdout();
    const auto expected_result = "Floating point: 2.1\n";
    EXPECT_EQ(expected_result, output);
}

TEST(sfinae, print_3)
{
    testing::internal::CaptureStdout();

    // NOTE: <float> is needed otherwise we get an error:
    // candidate template ignored: couldn't infer template argument 'T'
    print_3<float>(float_value);

    const auto output = testing::internal::GetCapturedStdout();
    const auto expected_result = "Floating point: 2.1\n";
    EXPECT_EQ(expected_result, output);
}
