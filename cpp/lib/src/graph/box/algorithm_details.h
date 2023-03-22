
#pragma once

#include <cstdint>

#include "edge.h"
#include "graph.h"
#include "list.h"

namespace box::details
{

template <GraphT Graph>
struct GraphSize;

template <NodeEdgesT Head, NodeEdgesT... Tail>
struct GraphSize<Graph<Head, Tail...>>
{
    static constexpr uint32_t size = GraphSize<Graph<Head>>::size + GraphSize<Graph<Tail...>>::size;
};

template <NodeEdgesT Head>
struct GraphSize<Graph<Head>>
{
    static constexpr uint32_t size = ListSize<typename Head::Edges>::size;
};

/*******************************************************/

template <GraphT Graph>
struct GraphOrder
{
    static constexpr uint32_t order = ListSize<typename Graph::AdjacencyList>::size;
};

/*******************************************************/

template <GraphT Graph, NodeT Node>
struct NodeDegree;

template <NodeT HeadNode, NodeT... Edges, NodeEdgesT... Tail, NodeT Node>
struct NodeDegree<Graph<NodeEdges<HeadNode, Edges...>, Tail...>, Node>
{
    static constexpr uint32_t degree = NodeDegree<Graph<Tail...>, Node>::degree;
};

template <NodeT HeadNode, NodeT... Edges, NodeEdgesT... Tail>
struct NodeDegree<Graph<NodeEdges<HeadNode, Edges...>, Tail...>, HeadNode>
{
    static constexpr uint32_t degree = ListSize<List<Edges...>>::size;
};

template <NodeT Node>
struct NodeDegree<Graph<>, Node>
{
    static constexpr uint32_t degree = 0;
};

/*******************************************************/

template <GraphT Graph, NodeT Node1, NodeT Node2, bool isDirected>
struct AdjacentNodes;

template <NodeT HeadNode,
          NodeT... Edges,
          NodeEdgesT... Tail,
          NodeT Node1,
          NodeT Node2,
          bool isDirected>
struct AdjacentNodes<Graph<NodeEdges<HeadNode, Edges...>, Tail...>, Node1, Node2, isDirected>
{
    static constexpr bool adjacent =
        AdjacentNodes<Graph<Tail...>, Node1, Node2, isDirected>::adjacent;
};

template <NodeT Node1, NodeT Node2, bool isDirected>
struct AdjacentNodes<Graph<>, Node1, Node2, isDirected>
{
    static constexpr bool adjacent = false;
};

// directed and undirected versions
template <NodeT HeadNode, NodeT... Edges, NodeEdgesT... Tail, NodeT Node2, bool isDirected>
struct AdjacentNodes<Graph<NodeEdges<HeadNode, Edges...>, Tail...>, HeadNode, Node2, isDirected>
{
    static constexpr bool adjacent = box::Contains<box::List<Edges...>, Node2>::value;
};

// undirected version
template <NodeT HeadNode, NodeT... Edges, NodeEdgesT... Tail, NodeT Node1>
struct AdjacentNodes<Graph<NodeEdges<HeadNode, Edges...>, Tail...>, Node1, HeadNode, false>
{
    static constexpr bool adjacent = box::Contains<box::List<Edges...>, Node1>::value;
};

/*******************************************************/

template <GraphT Graph, NodeT Node>
struct IsNodeInGraph;

template <NodeT HeadNode, NodeT... Edges, NodeEdgesT... Tail, NodeT Node>
struct IsNodeInGraph<Graph<NodeEdges<HeadNode, Edges...>, Tail...>, Node>
{
    static constexpr bool value = IsNodeInGraph<Graph<Tail...>, Node>::value;
};

template <NodeT HeadNode, NodeT... Edges, NodeEdgesT... Tail>
struct IsNodeInGraph<Graph<NodeEdges<HeadNode, Edges...>, Tail...>, HeadNode>
{
    static constexpr bool value = true;
};

template <NodeT Node>
struct IsNodeInGraph<Graph<>, Node>
{
    static constexpr bool value = false;
};

/*******************************************************/

template <GraphT Graph, NodeT Node1, NodeT Node2>
struct AreNodesConnected
{
    static constexpr bool value = false;
};

template <GraphT Graph, typename EdgesList, NodeT Node>
struct AreNodesConnectedList
{
    static constexpr bool value = false;
};

template <NodeT HeadNode, NodeT... Edges, NodeEdgesT... Tail, NodeT Node1, NodeT Node2>
struct AreNodesConnected<Graph<NodeEdges<HeadNode, Edges...>, Tail...>, Node1, Node2>
{
    static constexpr bool value =
        (IsNodeInGraph<Graph<Tail...>, Node1>::value)
            ? AreNodesConnected<Graph<Tail..., NodeEdges<HeadNode, Edges...>>, Node1, Node2>::value
            : false;
};

template <NodeT HeadNode, NodeT... Edges, NodeEdgesT... Tail, NodeT Node2>
struct AreNodesConnected<Graph<NodeEdges<HeadNode, Edges...>, Tail...>, HeadNode, Node2>
{
    static constexpr bool value =
        (Contains<List<Edges...>, Node2>::value)
            ? true
            : AreNodesConnectedList<Graph<Tail...>, List<Edges...>, Node2>::value;
};

template <GraphT Graph, NodeT FrontEdgeNode, NodeT... Edges, NodeT Node>
struct AreNodesConnectedList<Graph, List<FrontEdgeNode, Edges...>, Node>
{
    static constexpr bool value = AreNodesConnectedList<Graph, List<Edges...>, Node>::value ||
                                  AreNodesConnected<Graph, FrontEdgeNode, Node>::value;
};

/*******************************************************/

template <GraphT Graph>
struct GetNodes;

template <NodeT HeadNode, NodeT... Edges, NodeEdgesT... Tail>
struct GetNodes<Graph<NodeEdges<HeadNode, Edges...>, Tail...>>
{
    using Type = typename PushBack<typename GetNodes<Graph<Tail...>>::Type, HeadNode>::Type;
};

template <NodeT HeadNode, NodeT... Edges>
struct GetNodes<Graph<NodeEdges<HeadNode, Edges...>>>
{
    using Type = List<HeadNode>;
};

template <GraphT Graph, typename NodeList, std::size_t Size>
struct IsGraphConnectedNodes;

template <GraphT Graph, NodeT Head, NodeT... Tail, std::size_t Size>
struct IsGraphConnectedNodes<Graph, List<Head, Tail...>, Size>
{
    static constexpr bool value =
        (!AreNodesConnectedList<Graph, List<Tail...>, Head>::value)
            ? false
            : IsGraphConnectedNodes<Graph, List<Tail..., Head>, Size - 1>::value;
};

template <GraphT Graph, NodeT Head, NodeT... Tail>
struct IsGraphConnectedNodes<Graph, List<Head, Tail...>, /* Size */ 0>
{
    static constexpr bool value = true;
};

template <GraphT Graph>
struct IsGraphConnected
{
    using Nodes = typename GetNodes<Graph>::Type;
    static constexpr bool value = IsGraphConnectedNodes<Graph, Nodes, ListSize<Nodes>::size>::value;
};

/*******************************************************/

template <GraphT Graph, NodeT... Edges>
struct AreNodesInGraph
{
    static constexpr bool value = true;
};

template <GraphT Graph, NodeT HeadEdge, NodeT... Tail>
struct AreNodesInGraph<Graph, HeadEdge, Tail...>
{
    static constexpr bool value =
        (IsNodeInGraph<Graph, HeadEdge>::value) ? AreNodesInGraph<Graph, Tail...>::value : false;
};

template <GraphT Graph, size_t graph_order = GraphOrder<Graph>::order>
struct ValidateGraph;

template <NodeT HeadNode, NodeT... Edges, NodeEdgesT... Tail, size_t graph_order>
struct ValidateGraph<Graph<NodeEdges<HeadNode, Edges...>, Tail...>, graph_order>
{
    using ReversedGraph = Graph<Tail..., NodeEdges<HeadNode, Edges...>>;

    static constexpr bool value = (AreNodesInGraph<ReversedGraph, Edges...>::value)
                                      ? ValidateGraph<ReversedGraph, graph_order - 1>::value
                                      : false;
};

template <NodeT HeadNode, NodeT... Edges, NodeEdgesT... Tail>
struct ValidateGraph<Graph<NodeEdges<HeadNode, Edges...>, Tail...>, /* graph_order */ 0>
{
    static constexpr bool value = true;
};

} // namespace box::details
