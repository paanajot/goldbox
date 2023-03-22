
#pragma once

template <typename... Types>
struct ArgsCounter;

// NOTE: head and tail idiom
template <typename Head, typename... Tail>
struct ArgsCounter<Head, Tail...>
{
    static constexpr uint32_t value = 1 + ArgsCounter<Tail...>::value;
};

template <>
struct ArgsCounter<>
{
    static constexpr uint32_t value = 0;
};
