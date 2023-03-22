#include <gtest/gtest.h>
#include <stdint.h>

#include "args_counter.h"

TEST(args_counter, zero_args)
{
    constexpr uint32_t expected_zero_args = 0;
    EXPECT_EQ(expected_zero_args, ArgsCounter<>::value);
}

TEST(args_counter, three_args)
{
    constexpr uint32_t expected_three_args = 3;
    EXPECT_EQ(expected_three_args, (ArgsCounter<int, float, double>::value));
}
