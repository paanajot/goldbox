
#pragma once

#include <cstdint>

namespace box
{

template <uint32_t idx>
struct Node
{
    static constexpr uint32_t m_idx = idx;
};

/*******************************************************/

template <typename T>
struct IsNode : false_type
{ };

template <uint32_t idx>
struct IsNode<Node<idx>> : true_type
{ };

template <typename T>
concept NodeT = IsNode<T>::value;
} // namespace box
