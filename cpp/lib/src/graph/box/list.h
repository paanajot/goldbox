
#pragma once

#include <cstdint>

#include "node.h"

namespace box
{

template <typename... T>
struct List
{ };

/*******************************************************/

template <typename List>
struct ListSize;

template <>
struct ListSize<List<>>
{
    static constexpr uint32_t size = 0;
};

template <typename H, typename... T>
struct ListSize<List<H, T...>>
{
    static constexpr uint32_t size = 1 + ListSize<List<T...>>::size;
};

/*******************************************************/

template <typename List, typename Element>
struct Contains;

template <typename H, typename... T, typename Element>
struct Contains<List<H, T...>, Element>
{
    static constexpr bool value = Contains<List<T...>, Element>::value;
};

template <typename Element>
struct Contains<List<>, Element>
{
    static constexpr bool value = false;
};

template <typename H, typename... T>
struct Contains<List<H, T...>, H>
{
    static constexpr bool value = true;
};

/*******************************************************/

template <typename List, typename NewElement>
struct PushBack;

template <typename... Elements, typename NewElement>
struct PushBack<List<Elements...>, NewElement>
{
    using Type = List<Elements..., NewElement>;
};

} // namespace box
