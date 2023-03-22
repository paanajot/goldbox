
#include <gtest/gtest.h>

#include <cmath>
#include <complex>

#include "strategy/discriminant_strategy.h"

namespace
{
constexpr auto a = 1;
constexpr auto b = 2;
constexpr auto c = 3;
} // namespace

TEST(strategy, ordinary_discriminant_strategy)
{
    OrdinaryDiscriminantStrategy strategy;
    const QuadraticEquationSolver solver(strategy);

    auto [x1, x2] = solver.solve(a, b, c);
    EXPECT_FALSE(std::isnan(x1.real()));
    EXPECT_FALSE(std::isnan(x2.real()));
}

TEST(strategy, real_discriminant_strategy)
{
    RealDiscriminantStrategy strategy;
    const QuadraticEquationSolver solver(strategy);

    auto [x1, x2] = solver.solve(a, b, c);
    EXPECT_TRUE(std::isnan(x1.real()));
    EXPECT_TRUE(std::isnan(x2.real()));
}
