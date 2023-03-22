
#pragma once

#include <concepts>
#include <cstddef>

template <typename T>
concept incrementable = requires(T t)
{
    ++t;
};

template <typename T>
concept signed_constant = std::is_arithmetic_v<T> && std::is_unsigned_v<T>;

template <typename T, typename U>
concept plus_constant = requires(T t, U u)
{
    t + u;
};

template <typename T, typename U>
concept converter_constaints = incrementable<T> && plus_constant<T, U>;

template <signed_constant U, converter_constaints<U> T>
T converter_1(T initial, U constant)
{
    return ++initial + constant;
}

template <typename T, typename U>
requires signed_constant<U> && converter_constaints<T, U> T converter_2(T initial, U constant)
{
    return ++initial + constant;
}

template <typename T, typename U>
T converter_3(T initial, U constant) requires signed_constant<U> && converter_constaints<T, U>
{
    return ++initial + constant;
}

template <signed_constant U>
auto converter_4(converter_constaints<U> auto initial, U constant)
{
    return ++initial + constant;
}
