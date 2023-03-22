
#pragma once

#include <concepts>
#include <cstdint>

#include "box/type_traits.h"
#include "list.h"
#include "node.h"

namespace box
{

template <NodeT H, NodeT... T>
struct NodeEdges
{
    using Node = H;
    using Edges = List<T...>;
};

/*******************************************************/

template <typename NodeEdges, typename... NewEdges>
struct AddEdges;

template <typename H, typename... T, typename... NewEdges>
struct AddEdges<NodeEdges<H, T...>, NewEdges...>
{
    using Type = NodeEdges<H, T..., NewEdges...>;
};

/*******************************************************/

template <typename T>
struct IsNodeEdges : false_type
{ };

template <NodeT H, NodeT... T>
struct IsNodeEdges<NodeEdges<H, T...>> : true_type
{ };

template <typename T>
concept NodeEdgesT = IsNodeEdges<T>::value;

} // namespace box
