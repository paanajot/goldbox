
#pragma once

#include <cstddef>

namespace box
{
/*******************************************************/
/*
NOTE:
Template integral_constant defines a type, keyword constexpr defines a constant.
For example std::true_type is std::integral_constant<bool, true>.

One of the usage examples is tag-dispatching.
*/
template <bool T>
struct integral_constant
{
    using type = integral_constant<T>;
    constexpr static bool value = T;
};

using true_type = integral_constant<true>;
using false_type = integral_constant<false>;

/*******************************************************/
template <typename T1, typename T2>
struct is_same : false_type
{ };

template <typename T>
struct is_same<T, T> : true_type
{ };

template <typename T1, typename T2>
constexpr bool is_same_v = is_same<T1, T2>::value;

/*******************************************************/
template <typename T>
struct remove_cv
{
    using type = T;
};

template <typename T>
struct remove_cv<const T>
{
    using type = T;
};

template <typename T>
struct remove_cv<volatile T>
{
    using type = T;
};

template <typename T>
struct remove_cv<const volatile T>
{
    using type = T;
};

template <typename T>
using remove_cv_t = typename remove_cv<T>::type;

/*******************************************************/
template <typename T>
struct is_floating_point : false_type
{ };

template <>
struct is_floating_point<double> : true_type
{ };

template <>
struct is_floating_point<float> : true_type
{ };

template <>
struct is_floating_point<long double> : true_type
{ };

template <typename T>
constexpr bool is_floating_point_v = is_floating_point<T>::value;

/***********/

template <typename T>
struct is_floating_point2
    : integral_constant<is_same_v<T, double> || is_same_v<T, long double> || is_same_v<T, float>>
{ };

template <typename T>
constexpr bool is_floating_point_v2 = is_floating_point2<T>::value;

/*******************************************************/
template <typename T>
struct is_null_pointer : false_type
{ };

template <>
struct is_null_pointer<std::nullptr_t> : true_type
{ };

template <typename T>
constexpr bool is_null_pointer_v = is_null_pointer<T>::value;

/***********/

template <typename T>
struct is_null_pointer2 : is_same<T, std::nullptr_t>
{ };

template <typename T>
constexpr bool is_null_pointer_v2 = is_null_pointer<T>::value;

/*******************************************************/
template <bool, typename T = void>
struct enable_if
{ };

template <typename T>
struct enable_if<true, T>
{
    using type = T;
};

template <bool condition, typename T = void>
using enable_if_t = typename enable_if<condition, T>::type;

/*******************************************************/
template <bool B, class T, class F>
struct conditional
{
    using type = F;
};

template <class T, class F>
struct conditional<true, T, F>
{
    using type = T;
};

template <bool B, class T, class F>
using conditional_t = typename conditional<B, T, F>::type;

/*******************************************************/
template <typename T>
struct is_lvalue_reference : false_type
{ };

template <typename T>
struct is_lvalue_reference<T&> : true_type
{ };

template <typename T>
inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

/*******************************************************/
template <typename T>
struct is_rvalue_reference : false_type
{ };

template <typename T>
struct is_rvalue_reference<T&&> : true_type
{ };

template <typename T>
inline constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;

/*******************************************************/
template <typename T>
struct is_void : is_same<void, remove_cv_t<T>>
{ };

template <typename T>
inline constexpr bool is_void_v = is_void<T>::value;

/*******************************************************/
template <typename T>
struct is_pointer : false_type
{ };

template <typename T>
struct is_pointer<T*> : std::true_type
{ };

template <typename T>
struct is_pointer<T* const> : std::true_type
{ };

template <typename T>
struct is_pointer<T* volatile> : std::true_type
{ };

template <typename T>
struct is_pointer<T* const volatile> : std::true_type
{ };

template <typename T>
inline constexpr bool is_pointer_v = is_pointer<T>::value;
} // namespace box
