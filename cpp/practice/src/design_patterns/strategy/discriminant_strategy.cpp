#include "discriminant_strategy.h"

#include <cmath>
#include <complex>
#include <utility>

double OrdinaryDiscriminantStrategy::calculate_discriminant(double a, double b, double c) const
{
    return (b * b - 4 * a * c);
}

double RealDiscriminantStrategy::calculate_discriminant(double a, double b, double c) const
{
    const auto discriminant = (b * b - 4 * a * c);
    return (discriminant < 0) ? nan("negative discriminant") : discriminant;
}

QuadraticEquationSolver::QuadraticEquationSolver(DiscriminantStrategy& strategy)
    : m_strategy(strategy)
{ }

std::pair<std::complex<double>, std::complex<double>>
QuadraticEquationSolver::solve(double a, double b, double c) const
{
    const std::complex<double> discriminant{m_strategy.calculate_discriminant(a, b, c), 0};
    const auto sqrt_discriminant = sqrt(discriminant);
    const auto denominator = 2 * a;
    return {{(-b + sqrt_discriminant) / denominator}, {(-b - sqrt_discriminant) / denominator}};
}
