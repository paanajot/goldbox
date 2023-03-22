#include <gtest/gtest.h>

#include "facade/engine_facade.h"

TEST(facade, start_stop_engine)
{
    EngineFacade engine{};
    EXPECT_NO_THROW(engine.start_engine());
    EXPECT_NO_THROW(engine.stop_engine());
}
