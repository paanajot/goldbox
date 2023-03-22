
#pragma once

#include <iostream>

#include "box/type_traits.h"

/*
NOTE:
You cannot overload function that differ only in the template argument.

error: redefinition of ‘template<class T, class> void sfinae::print(T)’
template <typename T, typename = box::enable_if_t<!box::is_same_v<T, float>>>
void print(T t)
{
    std::cout << "Non floating point: " << t << std::endl;
}
*/

void print_1(int var)
{
    std::cout << "Integral: " << var << std::endl;
}

template <typename T, typename = box::enable_if_t<box::is_same_v<T, float>>>
void print_1(T t)
{
    std::cout << "Floating point: " << t << std::endl;
}

template <typename T>
box::enable_if_t<box::is_same_v<T, float>> print_2(T t)
{
    // cppcheck-suppress missingReturn; (false positive)
    std::cout << "Floating point: " << t << std::endl;
}

template <typename T>
void print_3(box::enable_if_t<box::is_same_v<T, float>, T> t)
{
    std::cout << "Floating point: " << t << std::endl;
}
