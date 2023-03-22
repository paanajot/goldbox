#include <gtest/gtest.h>
#include <string>

#include "crtp.h"

namespace
{
template <typename T>
std::string get_your_favourite_color(const Color<T>& color)
{
    return color.get_color();
}
} // namespace

TEST(crtp, static_interface)
{
    const std::string expected_red = "red";
    EXPECT_EQ(expected_red, get_your_favourite_color(RedColor()));

    const std::string expected_green = "green";
    EXPECT_EQ(expected_green, get_your_favourite_color(GreenColor()));
}
