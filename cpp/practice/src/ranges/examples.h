#pragma once
#ifndef __clang__ // not yet supported in clang

// clang-format off
#include <algorithm>
#include <numeric>
#include <ranges>
#include <vector>

#include <box/type_traits.h>
// clang-format on

template <typename Func>
auto min_element_example(Func get_values)
{
    const auto input_values = get_values();

    // You don't have to pass begin() && end() iterators anymore.
    const auto min_element_iter = std::ranges::min_element(input_values);

    // NOTE C++20: when you pass an rvalue range int an algorithm, std::ranges::dangling is returned
    // unless the input range meet enable_borrowed_range requirement, e.g. std::span, std::string_view
    const auto dangling_iter = std::ranges::min_element(get_values());
    static_assert(box::is_same_v<const std::ranges::dangling, decltype(dangling_iter)>);

    return *min_element_iter;
}

template <typename Func>
auto sum_of_squares_example_1(Func get_values)
{
    auto input_values = get_values();

    const auto square = [](const auto el) { return el * el; };

    // that's the old version of transforming
    // std::transform(input_values.begin(), input_values.end(), input_values.begin(), square);

    std::ranges::transform(input_values, input_values.begin(), square);

    // init_value has to be the same type as elements in the vector, otherwise implicit conversion would occur
    const typename decltype(input_values)::value_type init_value{};
    return std::accumulate(input_values.begin(), input_values.end(), init_value);
}

template <typename Func>
auto sum_of_squares_example_2(Func get_values)
{
    const auto input_values = get_values();

    const auto square = [](const auto el) { return el * el; };
    const auto square_view = input_values | std::ranges::views::transform(square);

    const typename decltype(input_values)::value_type init_value{};
    return std::accumulate(square_view.begin(), square_view.end(), init_value);
}
#endif
