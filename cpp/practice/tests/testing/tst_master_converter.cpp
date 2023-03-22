#include <gtest/gtest.h>

#include "master_converter.h"

TEST(converter, run)
{
    EXPECT_FALSE(MasterConverter().master_run());
}
