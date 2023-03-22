#include <box/unique_ptr.h> // NOLINT
#include <gtest/gtest.h>

#include "adapter/square.h"
#include "adapter/square_to_rectangle_adapter.h"

TEST(adapter, square_to_rectangle_adapter)
{
    const Square square{5};
    const box::unique_ptr rectangle(new SquareToRectangleAdapter(square));

    constexpr auto square_area = 25;
    EXPECT_EQ(rectangle->area(), square_area);
}
