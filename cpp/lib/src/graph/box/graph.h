
#pragma once

#include <cstdint>

#include "edge.h"
#include "list.h"
#include "node.h"

namespace box
{

template <NodeEdgesT... NodeEdges>
struct Graph
{
    using AdjacencyList = List<NodeEdges...>;
};

/*******************************************************/

template <typename T>
struct IsGraph : false_type
{ };

template <NodeEdgesT... NodeEdges>
struct IsGraph<Graph<NodeEdges...>> : true_type
{ };

template <typename T>
concept GraphT = IsGraph<T>::value;

} // namespace box
