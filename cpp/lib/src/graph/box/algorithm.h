
#pragma once

#include <cstdint>

#include "algorithm_details.h"

namespace box
{

// The size of a graph is its number of edges |E|
template <GraphT Graph>
consteval uint32_t get_graph_size()
{
    return details::GraphSize<Graph>::size;
}

// The order of a graph is its number of vertices |V|
template <GraphT Graph>
consteval uint32_t get_graph_order()
{
    return details::GraphOrder<Graph>::order;
}

// Degree of a vertex (node) of a graph is the number of edges that are incident to the vertex.
template <GraphT Graph, NodeT Node>
consteval uint32_t get_node_degree()
{
    return details::NodeDegree<Graph, Node>::degree;
}

// Are the following definitions correct?
// In a directed graph vertex v is adjacent to u, if there is an edge leaving v and coming to u.
// In an undirected graph, two vertices are said to be adjacent, if there is an edge between them.
template <GraphT Graph, NodeT Node1, NodeT Node2, bool isDirected = true>
consteval bool are_nodes_adjacent()
{
    return details::AdjacentNodes<Graph, Node1, Node2, isDirected>::adjacent;
}

// Nodes are connected if there is a directed path between them.
template <GraphT Graph, NodeT Node1, NodeT Node2>
consteval bool are_nodes_connected()
{
    return details::AreNodesConnected<Graph, Node1, Node2>::value;
}

// Checks if a graph consist of a given node.
template <GraphT Graph, NodeT Node>
consteval bool is_node_in_graph()
{
    return details::IsNodeInGraph<Graph, Node>::value;
}

// A graph is said to be connected if there is a path between every pair of vertex.
template <GraphT Graph>
consteval bool is_graph_connected()
{
    return details::IsGraphConnected<Graph>::value;
}

template <GraphT Graph>
consteval bool validate_graph()
{
    return details::ValidateGraph<Graph>::value;
}

} // namespace box
