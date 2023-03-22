#include <gtest/gtest.h>

#include "sum.h"

TEST(variadic_templates, sum)
{
    EXPECT_EQ(sum(1, 2, 3), 6);
    EXPECT_DOUBLE_EQ(sum(1.2, 2.3, 3.4), 6.9);
}

TEST(variadic_templates, sum2)
{
    EXPECT_EQ(sum2(2, 3, 4), 9);
    EXPECT_DOUBLE_EQ(sum2(0.2, 1.2, 2.3, 3.4), 7.1);
}

TEST(variadic_templates, sum3)
{
    EXPECT_EQ(sum3(3, 4, 5), 12);
    EXPECT_DOUBLE_EQ(sum3(2.3, 3.4), 5.7);
}
