#pragma once

#include <list>
#include <span>
#include <vector>

#include "point.h"

// C++20 NOTE: designated initialization - each designator must name a direct non-static data member of T,
// and all designators used in the expression must appear in the same order as the data members of T.
constexpr Point point{.m_x = 1, .m_y = 2, .m_z = 3};

// C++20 NOTE: consteval specifier - keyword that specifies immediate function. In contrast to constexpr functions,
// they cannot be called at runtime.
consteval int add(int lhs, int rhs)
{
    return lhs + rhs;
}
constexpr int result = add(1, 2);

// C++20 NOTE: constinit - ensures that the variable is initialized at compile-time
// and solves 'static initialization order fiasco' problem.
// https://isocpp.org/wiki/faq/ctors#static-init-order
constexpr int foo()
{
    constexpr int random_number = 9;
    return random_number;
}
constinit int foo_value = foo();

// C++20 NOTE: range-based for with initializer
void boo()
{
    const std::vector<int> my_collection{1, 2, 3, 4, 5, 6};
    for(const int offset = 4; const auto number : my_collection)
    {
        [[maybe_unused]] const auto result = number + offset;
    }
}

// C++20 NOTE: Attibutes likely & unlikely - guides compiler about more likely/unlikely code path
void meow(bool flag)
{
    if(flag) [[likely]]
    {
        /* do something */
    }
    else [[unlikely]]
    {
        /* do something else */
    }
}

// C++20 NOTE: non-type template parameters
// since c++20 auto can be used to deduce a type
template <auto... n>
struct NonTypeClass
{ };

// since c++20 floating-point types are allowed
// NonTypeClass<2.5> foo_1; // TODO: install new version of clang, not yet supported in the current version

// variadic as a non-type argument
NonTypeClass<'C', 0, 2L, nullptr> foo_2;

// class as a non-type argument
NonTypeClass<point> foo_3;

// C++20 NOTE: std::span sometimes is called a view, is not an owner,
// it is just an abstraction over contiguous sequence of data.
// It comes with a useful set of methods.
void write([[maybe_unused]] std::span<int> container)
{
    /* do sth e.g. for(auto e : container) */
}

void span_call()
{
    // it automatically deduces the size of a plain container
    int c_1[]{1, 2, 3, 4};
    write(c_1);

    std::vector c_2{1, 2, 3, 4};
    write(c_2);

    std::array c_3{1, 2, 3, 4};
    write(c_3);

    // std::list doesn't work, it is not a contiguous sequence of data
    // std::list c_4{1, 2, 3, 4};
    // write(c_4);
}

// Use span to preserve size information
void span_foo(int[]); // not recommended
void span_foo(int*); // not recommended
void span_foo(std::span<int>); // good
