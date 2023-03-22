#include <gtest/gtest.h>
#include <string>

#include "box/tuple.h"

namespace
{
constexpr int val_1 = 1;
constexpr double val_2 = 2.1;
const std::string val_3 = "my_string";
} // namespace

TEST(tuple, get_with_three_elements)
{
    box::tuple<int, double, std::string> tuple{val_1, val_2, val_3};
    EXPECT_EQ(val_1, box::get<0>(tuple));
    EXPECT_EQ(val_2, box::get<1>(tuple));
    EXPECT_EQ(val_3, box::get<2>(tuple));
}

TEST(tuple, make_tuple)
{
    const auto tuple = box::make_tuple(val_1, val_2, val_3);
    EXPECT_EQ(val_1, box::get<0>(tuple));
    EXPECT_EQ(val_2, box::get<1>(tuple));
    EXPECT_EQ(val_3, box::get<2>(tuple));
}

TEST(tuple, tie)
{
    std::string my_string = "my_string";
    const auto tuple = box::tie(val_1, val_2, my_string);
    EXPECT_EQ(val_1, box::get<0>(tuple));
    EXPECT_EQ(val_2, box::get<1>(tuple));
    EXPECT_EQ(my_string, box::get<2>(tuple));

    my_string = "my_new_string";
    EXPECT_EQ(my_string, box::get<2>(tuple));
}
