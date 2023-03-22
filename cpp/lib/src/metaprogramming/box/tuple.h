
#pragma once

#include <cstdint>

namespace box
{
template <typename... Types>
class tuple
{ };

template <typename Head, typename... Tail>
class tuple<Head, Tail...>
{
public:
    explicit tuple(const Head& head, const Tail&... tail);

    Head m_head;
    tuple<Tail...> m_tail;
};

template <typename... Types>
auto make_tuple(Types&&... args);

template <typename... Types>
auto tie(Types&... args);

template <uint32_t idx, typename... Types>
auto get(box::tuple<Types...> tuple);

} // namespace box

/************************ IMPL ************************/
#include "tuple_impl.h"
