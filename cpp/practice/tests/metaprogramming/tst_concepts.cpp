#include <gtest/gtest.h>
#include <stdint.h>

#include "concepts.h"

namespace
{
constexpr uint32_t initial = 10;
constexpr uint16_t constant = 6;
constexpr uint32_t expected_value = 17;
} // namespace

TEST(concepts, converter_1)
{

    const auto value = converter_1(initial, constant);
    EXPECT_EQ(value, expected_value);
}

TEST(concepts, converter_2)
{
    const auto value = converter_2(initial, constant);
    EXPECT_EQ(value, expected_value);
}

TEST(concepts, converter_3)
{
    const auto value = converter_3(initial, constant);
    EXPECT_EQ(value, expected_value);
}

TEST(concepts, converter_4)
{
    const auto value = converter_4(initial, constant);
    EXPECT_EQ(value, expected_value);
}
