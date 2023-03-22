
#pragma once

#include <complex>
#include <utility>

// https://www.udemy.com/course/patterns-cplusplus/
// https://sourcemaking.com/design_patterns/strategy
// NOTE: Define a family of algorithms, encapsulate each one, and make them interchangeable.
struct DiscriminantStrategy
{
    virtual double calculate_discriminant(double a, double b, double c) const = 0;
};

struct OrdinaryDiscriminantStrategy : DiscriminantStrategy
{
    double calculate_discriminant(double a, double b, double c) const override;
};

struct RealDiscriminantStrategy : DiscriminantStrategy
{
    double calculate_discriminant(double a, double b, double c) const override;
};

class QuadraticEquationSolver
{
    DiscriminantStrategy& m_strategy;

public:
    explicit QuadraticEquationSolver(DiscriminantStrategy& strategy);

    std::pair<std::complex<double>, std::complex<double>> solve(double a, double b, double c) const;
};
