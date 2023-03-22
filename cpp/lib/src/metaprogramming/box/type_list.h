
#pragma once

#include <iostream>

#include "type_traits.h"

namespace box
{
template <typename... Elements>
struct TypeList
{ };

/*************** PopFront ***************/
template <typename T>
struct PopFrontT;

template <typename Head, typename... Tail>
struct PopFrontT<TypeList<Head, Tail...>>
{
    using Type = TypeList<Tail...>;
};

template <typename List>
using PopFront = typename PopFrontT<List>::Type;

/*************** Front ***************/
template <typename T>
struct FrontT;

template <typename Head, typename... Tail>
struct FrontT<TypeList<Head, Tail...>>
{
    using Type = Head;
};

template <typename List>
using Front = typename FrontT<List>::Type;

/*************** NthElement ***************/
template <typename List, size_t N>
struct NthElementT : public NthElementT<PopFront<List>, N - 1>
{ };

template <typename List>
struct NthElementT<List, 0> : public FrontT<List>
{ };

template <typename List, size_t N>
using NthElement = typename NthElementT<List, N>::Type;

/*************** PushBack ***************/
template <typename Elements, typename T>
struct PushBackT;

template <typename... Elements, typename T>
struct PushBackT<TypeList<Elements...>, T>
{
    using Type = TypeList<Elements..., T>;
};

template <typename List, typename T>
using PushBack = typename PushBackT<List, T>::Type;

/***************** Back *****************/
template <typename List>
struct BackT;

template <typename... Elements>
struct BackT<TypeList<Elements...>>
{
    using Type = NthElement<TypeList<Elements...>, sizeof...(Elements) - 1>;
};

template <typename List>
using Back = typename BackT<List>::Type;

/*************** IsEmpy ***************/
template <typename List>
struct IsEmptyV
{
    static constexpr bool value = false;
};

template <>
struct IsEmptyV<TypeList<>>
{
    static constexpr bool value = true;
};

template <typename List>
static constexpr bool is_empty = IsEmptyV<List>::value;

/*************** Reverse ***************/
template <typename List, bool = is_empty<List>>
struct ReverseListT : PushBackT<typename ReverseListT<PopFront<List>>::Type, Front<List>>
{ };

template <typename List>
struct ReverseListT<List, true>
{
    using Type = List;
};

template <typename List>
using ReverseList = typename ReverseListT<List>::Type;

/*************** LargestType ***************/
template <typename List, bool = is_empty<List>>
struct LargestTypeT
{
    using FrontType = Front<List>;
    using BestType = typename LargestTypeT<PopFront<List>>::Type;
    using Type = box::conditional_t<(sizeof(FrontType) > sizeof(BestType)), FrontType, BestType>;
};

template <typename List>
struct LargestTypeT<List, true>
{
    using Type = char;
};

template <typename List>
using LargestType = typename LargestTypeT<List>::Type;

/*******************************************************/

} // namespace box
