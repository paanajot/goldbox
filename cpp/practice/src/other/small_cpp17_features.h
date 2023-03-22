#pragma once

#include "point.h"

#include <cstddef>

// C++17 NOTE: A constexpr specifier used in a function or static data member
// (since C++17) declaration implies inline.
constexpr Point local_point{.m_x = 1, .m_y = 2, .m_z = 3};

// C++17 NOTE: structured binding - decomposes packed structures
const auto& [x, y, z] = local_point;

// C++17 NOTE: nested namespace definition
namespace A::B::C
{

}

// C++17 NOTE: inline variable can be defined in multiple translation units
// without violating the 'One Definition Rule'. The linker merges all definitions
// into a single object in the final program.
struct Foo
{
    inline static const int inline_variable = 123;
};

// C++17 NOTE: guaranteed copy elision (RVO) - omits copy and move constructors,
// the objects are constructed directly into the storage.
Point get_default_point()
{
    return Point{};
}

void call()
{
    // only one call to default constructor of Point
    // C++17 NOTE: maybe_unused attribute - compiler will not warn you about unused variable
    [[maybe_unused]] const auto default_point = get_default_point();
}

// C++17 NOTE: constexpr lambda expressions
constexpr auto get_number = [](const auto& number) { return number; };

// C++17 NOTE: static asserts without msg
static_assert(get_number(2) == 2);

// C++17 NOTE: fallthrough attribute - inidcates that a case in switch can fallthrough
// (compiler will not raise a warning)
int switch_call(int id)
{
    switch(id)
    {
    case 1:
        ++id;
        [[fallthrough]];
    case 2:
        id += 2;
        break;
    default:
        id += 3;
        break;
    }
    return id;
}

// C++17 NOTE: nodiscard - reports warning if return value is discarded, applies to functions and classes
struct [[nodiscard]] NodiscardClass
{
    int foo()
    {
        return 1;
    }
};

// [[nodiscard]] is not needed here since the NodiscardClass class is already nodiscard
[[nodiscard]] NodiscardClass nodiscard_call()
{
    return {};
}

// C++17 NOTE: std::byte - it is preferable as a view of bits instead of char and uint8_t.
// It should be used by a programmer who needs to work with raws memory.
void byte_call()
{
    // NOLINTBEGIN [readability-magic-numbers]
    std::byte b{0b11110000};
    b |= std::byte{0b11110000};
    b ^= std::byte{0b11111111};
    b >>= 1;
    // NOLINTEND
}
