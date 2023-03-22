#ifndef __clang__

// clang-format off
#include <gtest/gtest.h>
#include <vector>

#include "examples.h"
// clang-format on

namespace
{
std::vector<int> get_input_1()
{
    return {1, 2, 3, 4, 5}; // NOLINT
}
constexpr auto expected_min_element_1 = 1;
constexpr auto expected_sum_of_squares_1 = 55;

std::vector<double> get_input_2()
{
    return {-1.2, -21., 3.2, 4., 12.52}; // NOLINT
}

constexpr auto expected_min_element_2 = -21.;
constexpr auto expected_sum_of_squares_2 = 625.4304;
} // namespace

TEST(ranges_examples, min_element_example)
{
    const auto result_1 = min_element_example(get_input_1);
    EXPECT_EQ(result_1, expected_min_element_1);

    const auto result_2 = min_element_example(get_input_2);
    EXPECT_EQ(result_2, expected_min_element_2);
}

TEST(ranges_examples, sum_of_squares_example_1)
{
    const auto result_1 = sum_of_squares_example_1(get_input_1);
    EXPECT_EQ(result_1, expected_sum_of_squares_1);

    const auto result_2 = sum_of_squares_example_1(get_input_2);
    EXPECT_DOUBLE_EQ(result_2, expected_sum_of_squares_2);
}

TEST(ranges_examples, sum_of_squares_example_2)
{
    const auto result_1 = sum_of_squares_example_2(get_input_1);
    EXPECT_EQ(result_1, expected_sum_of_squares_1);

    const auto result_2 = sum_of_squares_example_2(get_input_2);
    EXPECT_DOUBLE_EQ(result_2, expected_sum_of_squares_2);
}
#endif
