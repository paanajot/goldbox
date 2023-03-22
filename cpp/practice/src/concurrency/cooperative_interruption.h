
#pragma once

#include <stop_token>

namespace concurrency
{

class NumberPrinter
{
public:
    static void print_even_numbers(std::stop_token stop_token);
    static void print_odd_numbers(std::stop_token stop_token, std::stop_source stop_source);

    void main() const;
};

} // namespace concurrency
