#include <gtest/gtest.h>

#include "box/algorithm.h"
#include "box/edge.h"
#include "box/graph.h"
#include "box/list.h"
#include "box/node.h"

namespace
{
// NOLINTBEGIN
using N0 = box::Node<0>;
using N1 = box::Node<1>;
using N2 = box::Node<2>;
using N3 = box::Node<3>;
using N4 = box::Node<4>;
using N5 = box::Node<5>;
using N6 = box::Node<6>;
// NOLINTEND

using E0 = box::NodeEdges<N0, N1>;
using E1 = box::NodeEdges<N1, N2>;
using E2 = box::NodeEdges<N2, N3>;
using E3 = box::NodeEdges<N3>;

using E4 = box::NodeEdges<N0, N1, N2, N3>;
using E5 = box::NodeEdges<N1, N2, N1>;
using E6 = box::NodeEdges<N2, N3, N1>;
using E7 = box::NodeEdges<N3>;
using E8 = box::NodeEdges<N4>;

using G0 = box::Graph<E0, E1, E2, E3>;
using G1 = box::Graph<E4, E5, E6, E7, E8>;

static_assert(box::validate_graph<G0>());
static_assert(box::validate_graph<G1>());
} // namespace

TEST(graph, graph_size_1)
{
    constexpr auto expected_size = 3;
    EXPECT_EQ(expected_size, box::get_graph_size<G0>());
}

TEST(graph, graph_size_2)
{
    constexpr auto expected_size = 7;
    EXPECT_EQ(expected_size, box::get_graph_size<G1>());
}

TEST(graph, graph_order_1)
{
    constexpr auto expected_order = 4;
    EXPECT_EQ(expected_order, box::get_graph_order<G0>());
}

TEST(graph, graph_order_2)
{
    constexpr auto expected_order = 5;
    EXPECT_EQ(expected_order, box::get_graph_order<G1>());
}

TEST(graph, add_edge_to_empty_list)
{
    using G1_1 = box::Graph<E0, E1, E8>;
    constexpr auto expected_size_1 = 2;
    EXPECT_EQ(expected_size_1, box::get_graph_size<G1_1>());

    using E8 = box::AddEdges<E8, N3>::Type;
    using G1_2 = box::Graph<E0, E1, E8>;

    constexpr auto expected_size_2 = 3;
    EXPECT_EQ(expected_size_2, box::get_graph_size<G1_2>());
}

TEST(graph, add_edge_to_non_empty_list)
{
    using G1_1 = box::Graph<E0, E1, E6>;
    constexpr auto expected_size_1 = 4;
    EXPECT_EQ(expected_size_1, box::get_graph_size<G1_1>());

    using E6 = box::AddEdges<E6, N3>::Type;
    using G1_2 = box::Graph<E0, E1, E6>;

    constexpr auto expected_size_2 = 5;
    EXPECT_EQ(expected_size_2, box::get_graph_size<G1_2>());
}

TEST(graph, add_edges)
{
    using G1_1 = box::Graph<E0, E1, E8>;
    constexpr auto expected_size_1 = 2;
    EXPECT_EQ(expected_size_1, box::get_graph_size<G1_1>());

    using E8 = box::AddEdges<E8, N3, N4, N1, N2>::Type;
    using G1_2 = box::Graph<E0, E1, E8>;

    constexpr auto expected_size_2 = 6;
    EXPECT_EQ(expected_size_2, box::get_graph_size<G1_2>());
}

TEST(graph, vertex_degree)
{
    constexpr auto expected_degree_0 = 1;
    EXPECT_EQ(expected_degree_0, (box::get_node_degree<G0, N0>()));

    constexpr auto expected_degree_1 = 1;
    EXPECT_EQ(expected_degree_1, (box::get_node_degree<G0, N1>()));

    constexpr auto expected_degree_2 = 1;
    EXPECT_EQ(expected_degree_2, (box::get_node_degree<G0, N2>()));

    constexpr auto expected_degree_3 = 0;
    EXPECT_EQ(expected_degree_3, (box::get_node_degree<G0, N3>()));

    constexpr auto expected_degree_4 = 3;
    EXPECT_EQ(expected_degree_4, (box::get_node_degree<G1, N0>()));
}

TEST(graph, adjacent_nodes)
{
    EXPECT_TRUE((box::are_nodes_adjacent<G0, N0, N1>()));

    constexpr bool directed = true;
    EXPECT_FALSE((box::are_nodes_adjacent<G0, N1, N0, directed>()));
    EXPECT_TRUE((box::are_nodes_adjacent<G0, N1, N0, !directed>()));

    EXPECT_FALSE((box::are_nodes_adjacent<G0, N3, N4>()));
    EXPECT_FALSE((box::are_nodes_adjacent<G0, N4, N0>()));
    EXPECT_FALSE((box::are_nodes_adjacent<G0, N0, N4>()));

    EXPECT_FALSE((box::are_nodes_adjacent<G0, N3, N4, !directed>()));
    EXPECT_FALSE((box::are_nodes_adjacent<G0, N4, N0, !directed>()));
    EXPECT_FALSE((box::are_nodes_adjacent<G0, N0, N4, !directed>()));
}

TEST(graph, are_nodes_connected)
{
    EXPECT_TRUE((box::are_nodes_connected<G0, N0, N1>()));
    EXPECT_TRUE((box::are_nodes_connected<G0, N1, N2>()));
    EXPECT_TRUE((box::are_nodes_connected<G0, N2, N3>()));

    EXPECT_TRUE((box::are_nodes_connected<G0, N0, N2>()));
    EXPECT_TRUE((box::are_nodes_connected<G0, N0, N3>()));

    EXPECT_TRUE((box::are_nodes_connected<G0, N1, N3>()));

    EXPECT_FALSE((box::are_nodes_connected<G0, N0, N4>()));
    EXPECT_FALSE((box::are_nodes_connected<G0, N1, N4>()));
    EXPECT_FALSE((box::are_nodes_connected<G0, N2, N4>()));
    EXPECT_FALSE((box::are_nodes_connected<G0, N3, N4>()));

    EXPECT_FALSE((box::are_nodes_connected<G0, N4, N4>()));
    EXPECT_FALSE((box::are_nodes_connected<G0, N4, N5>()));

    EXPECT_TRUE((box::are_nodes_connected<G1, N0, N1>()));
    EXPECT_TRUE((box::are_nodes_connected<G1, N0, N3>()));
    EXPECT_FALSE((box::are_nodes_connected<G1, N0, N4>()));
}

TEST(graph, is_node_in_graph)
{
    EXPECT_TRUE((box::is_node_in_graph<G0, N0>()));
    EXPECT_TRUE((box::is_node_in_graph<G0, N1>()));
    EXPECT_TRUE((box::is_node_in_graph<G0, N2>()));
    EXPECT_TRUE((box::is_node_in_graph<G0, N3>()));

    EXPECT_FALSE((box::is_node_in_graph<G0, N4>()));
    EXPECT_FALSE((box::is_node_in_graph<G0, N5>()));
    EXPECT_FALSE((box::is_node_in_graph<G0, N6>()));

    EXPECT_TRUE((box::is_node_in_graph<G1, N0>()));
    EXPECT_TRUE((box::is_node_in_graph<G1, N1>()));
    EXPECT_TRUE((box::is_node_in_graph<G1, N2>()));
    EXPECT_TRUE((box::is_node_in_graph<G1, N4>()));

    EXPECT_FALSE((box::is_node_in_graph<G1, N5>()));
    EXPECT_FALSE((box::is_node_in_graph<G1, N6>()));
}

TEST(graph, is_graph_connected)
{
    using E0 = box::NodeEdges<N0, N1>;
    using E1 = box::NodeEdges<N1, N2>;
    using E2 = box::NodeEdges<N2, N0>;

    using E4 = box::NodeEdges<N3, N4, N6>;
    using E5 = box::NodeEdges<N4, N5>;
    using E6 = box::NodeEdges<N5, N3>;
    using E8 = box::NodeEdges<N6, N4>;

    using ConnectedGraph1 = box::Graph<E0, E1, E2>;
    using ConnectedGraph2 = box::Graph<E4, E5, E6, E8>;

    EXPECT_TRUE((box::are_nodes_connected<ConnectedGraph1, N0, N1>()));
    EXPECT_TRUE((box::are_nodes_connected<ConnectedGraph1, N1, N2>()));
    EXPECT_TRUE((box::are_nodes_connected<ConnectedGraph1, N2, N0>()));

    EXPECT_TRUE((box::is_graph_connected<ConnectedGraph1>()));
    EXPECT_TRUE((box::is_graph_connected<ConnectedGraph2>()));

    EXPECT_FALSE((box::is_graph_connected<G0>()));
    EXPECT_FALSE((box::is_graph_connected<G1>()));
}

TEST(graph, validate_graph)
{
    using E0 = box::NodeEdges<N0, N1>;
    using E1 = box::NodeEdges<N1, N2>;
    using E2 = box::NodeEdges<N2, N3>;
    using E3 = box::NodeEdges<N3>;

    // connects to a node that it nos included in the graph
    using E4_invalid = box::NodeEdges<N0, N1, N2, N3, N5>;
    using E4 = box::NodeEdges<N0, N1, N2, N3>;
    using E5 = box::NodeEdges<N1, N2, N1>;
    using E6 = box::NodeEdges<N2, N3, N1>;
    using E7 = box::NodeEdges<N3>;
    using E8 = box::NodeEdges<N4>;

    using G0 = box::Graph<E0, E1, E2, E3>;
    using G1 = box::Graph<E4, E5, E6, E7, E8>;

    using G2 = box::Graph<E0, E1, E2>; // E3 is missing
    using G3 = box::Graph<E4_invalid, E5, E6, E7, E8>;

    static_assert(box::validate_graph<G0>());
    static_assert(box::validate_graph<G1>());

    static_assert(!box::validate_graph<G2>());
    static_assert(!box::validate_graph<G3>());
}

TEST(list, contains)
{
    using NodeList = box::List<N0, N1, N2>;
    using EmptyList = box::List<>;

    EXPECT_TRUE((box::Contains<NodeList, N0>::value));
    EXPECT_TRUE((box::Contains<NodeList, N1>::value));
    EXPECT_TRUE((box::Contains<NodeList, N2>::value));

    EXPECT_FALSE((box::Contains<NodeList, N3>::value));
    EXPECT_FALSE((box::Contains<EmptyList, N3>::value));
}

TEST(list, push_back)
{
    using NodeList = box::List<N0, N1, N2>;
    using EmptyList = box::List<>;

    using NewNodeList = box::PushBack<NodeList, N3>::Type;
    using NonEmptyList = box::PushBack<EmptyList, N0>::Type;

    EXPECT_FALSE((box::Contains<NodeList, N3>::value));
    EXPECT_TRUE((box::Contains<NewNodeList, N3>::value));

    EXPECT_FALSE((box::Contains<EmptyList, N0>::value));
    EXPECT_TRUE((box::Contains<NonEmptyList, N0>::value));
}
