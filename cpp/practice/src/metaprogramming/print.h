
#pragma once

#include <iostream>

/*******************************************************/

void print1()
{
    std::cout << '\n';
}

template <typename T, typename... Types>
void print1(T arg, Types... args)
{
    std::cout << arg << " ";
    print1(args...);
}

/*******************************************************/

template <typename T>
void print2(T arg)
{
    std::cout << arg << " ";
}

template <typename T, typename... Types>
void print2(T arg, Types... args)
{
    print2(arg);
    print2(args...);
    if(sizeof...(Types) == 1)
    {
        std::cout << '\n';
    }
}

/*******************************************************/

template <typename T, typename... Types>
void print3(T arg, Types... args)
{
    std::cout << arg << " ";
    if constexpr(sizeof...(Types) > 0)
    {
        print3(args...);
    }
    else
    {
        std::cout << '\n';
    }
}

/*******************************************************/

template <typename... Types>
void print4(Types... args)
{
    const auto print_helper = [](const auto& arg) { std::cout << arg << " "; };
    (print_helper(args), ...);
    std::cout << '\n';
}
