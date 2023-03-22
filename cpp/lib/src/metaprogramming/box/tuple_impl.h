
#pragma once

#include <cstdint>
#include <utility>

namespace box
{

template <typename Head, typename... Tail>
tuple<Head, Tail...>::tuple(const Head& head, const Tail&... tail)
    : m_head(head)
    , m_tail(tail...)
{ }

template <typename... Types>
auto make_tuple(Types&&... args)
{
    return box::tuple<Types...>(std::forward<Types>(args)...);
}

template <typename... Types>
auto tie(Types&... args)
{
    return box::tuple<Types&...>(args...);
}

template <uint32_t idx>
class TupleGet
{
public:
    template <typename Head, typename... Tail>
    static auto apply(box::tuple<Head, Tail...> tuple)
    {
        return TupleGet<idx - 1>::apply(box::tuple<Tail...>{tuple.m_tail});
    }
};

template <>
class TupleGet<0>
{
public:
    template <typename Head, typename... Tail>
    static auto apply(box::tuple<Head, Tail...> tuple)
    {
        return tuple.m_head;
    }
};

template <uint32_t idx, typename... Types>
auto get(box::tuple<Types...> tuple)
{
    return TupleGet<idx>::apply(tuple);
}
} // namespace box
