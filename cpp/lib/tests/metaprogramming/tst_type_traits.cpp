#include <gtest/gtest.h>
#include <string>
#include <type_traits>

#include "box/type_traits.h"

namespace
{
const std::string false_type_str = "false_type_str";
const std::string true_type_str = "true_type_str";

template <typename T>
std::string use_impl(const T&, box::false_type)
{
    return false_type_str;
}

template <typename T>
std::string use_impl(const T&, box::true_type)
{
    return true_type_str;
}

template <typename T>
std::string use(const T& v)
{
    return use_impl(v, typename box::is_same<T, float>::type{});
}
} // namespace

TEST(type_traits, is_null_pointer)
{
    EXPECT_TRUE(box::is_null_pointer_v<decltype(nullptr)>);

    EXPECT_FALSE(box::is_null_pointer_v<float>);
    EXPECT_FALSE(box::is_null_pointer_v<double>);
    EXPECT_FALSE(box::is_null_pointer_v<int>);
}

TEST(type_traits, is_null_pointer2)
{
    EXPECT_TRUE(box::is_null_pointer_v2<decltype(nullptr)>);

    EXPECT_FALSE(box::is_null_pointer_v2<float>);
    EXPECT_FALSE(box::is_null_pointer_v2<double>);
    EXPECT_FALSE(box::is_null_pointer_v2<int>);
}

TEST(type_traits, is_floating_point)
{
    EXPECT_TRUE(box::is_floating_point_v<double>);
    EXPECT_TRUE(box::is_floating_point_v<long double>);
    EXPECT_TRUE(box::is_floating_point_v<float>);

    EXPECT_FALSE(box::is_floating_point_v<int>);
    EXPECT_FALSE(box::is_floating_point_v<bool>);
    EXPECT_FALSE(box::is_floating_point_v<std::string>);
}

TEST(type_traits, is_floating_point2)
{
    EXPECT_TRUE(box::is_floating_point_v2<double>);
    EXPECT_TRUE(box::is_floating_point_v2<long double>);
    EXPECT_TRUE(box::is_floating_point_v2<float>);

    EXPECT_FALSE(box::is_floating_point_v2<int>);
    EXPECT_FALSE(box::is_floating_point_v2<bool>);
    EXPECT_FALSE(box::is_floating_point_v2<std::string>);
}

TEST(type_traits, is_same)
{
    EXPECT_TRUE((box::is_same_v<double, double>));
    EXPECT_TRUE((box::is_same_v<int, int>));

    EXPECT_FALSE((box::is_same_v<double, int>));
    EXPECT_FALSE((box::is_same_v<double, float>));
    EXPECT_FALSE((box::is_same_v<double, bool>));
}

TEST(type_traits, enable_if)
{
    EXPECT_EQ(typeid(double), typeid(box::enable_if_t<true, double>));
    EXPECT_EQ(typeid(int), typeid(box::enable_if_t<true, int>));
    EXPECT_EQ(typeid(void), typeid(box::enable_if_t<true>));
}

TEST(type_traits, tag_dispatching)
{
    EXPECT_EQ(use(1.f), true_type_str);
    EXPECT_EQ(use(true), false_type_str);
}

TEST(type_traits, conditional)
{
    using TrueType = box::conditional_t<true, int, double>;
    using FalseType = box::conditional_t<false, int, double>;
    EXPECT_EQ(typeid(int), typeid(TrueType));
    EXPECT_EQ(typeid(double), typeid(FalseType));
}

TEST(type_traits, is_lvalue_reference)
{
    EXPECT_FALSE(box::is_lvalue_reference_v<int>);
    EXPECT_FALSE(box::is_lvalue_reference_v<int&&>);
    EXPECT_TRUE(box::is_lvalue_reference_v<int&>);
    EXPECT_TRUE(box::is_lvalue_reference_v<const int&>);
}

TEST(type_traits, is_rvalue_reference)
{
    EXPECT_FALSE(box::is_rvalue_reference_v<int>);
    EXPECT_FALSE(box::is_rvalue_reference_v<int&>);
    EXPECT_TRUE(box::is_rvalue_reference_v<int&&>);
    EXPECT_TRUE(box::is_rvalue_reference_v<const int&&>);
}

TEST(type_traits, remove_cv)
{
    EXPECT_EQ(typeid(box::remove_cv_t<void>), typeid(std::remove_cv_t<void>));
    EXPECT_EQ(typeid(box::remove_cv_t<const void>), typeid(std::remove_cv_t<const void>));
    EXPECT_EQ(typeid(box::remove_cv_t<volatile void>), typeid(std::remove_cv_t<volatile void>));
    EXPECT_EQ(typeid(box::remove_cv_t<const volatile void>),
              typeid(std::remove_cv_t<const volatile void>));
    EXPECT_EQ(typeid(box::remove_cv_t<void*>), typeid(std::remove_cv_t<void*>));
    EXPECT_EQ(typeid(box::remove_cv_t<float>), typeid(std::remove_cv_t<float>));
    EXPECT_EQ(typeid(box::remove_cv_t<int>), typeid(std::remove_cv_t<int>));
}

TEST(type_traits, is_void)
{
    EXPECT_EQ(box::is_void_v<void>, std::is_void_v<void>);
    EXPECT_EQ(box::is_void_v<const void>, std::is_void_v<const void>);
    EXPECT_EQ(box::is_void_v<volatile void>, std::is_void_v<volatile void>);
    EXPECT_EQ(box::is_void_v<const volatile void>, std::is_void_v<const volatile void>);
    EXPECT_EQ(box::is_void_v<void*>, std::is_void_v<void*>);
    EXPECT_EQ(box::is_void_v<float>, std::is_void_v<float>);
    EXPECT_EQ(box::is_void_v<int>, std::is_void_v<int>);
}

TEST(type_traits, is_pointer)
{
    EXPECT_EQ(box::is_pointer_v<void>, std::is_pointer_v<void>);
    EXPECT_EQ(box::is_pointer_v<void*>, std::is_pointer_v<void*>);
    EXPECT_EQ(box::is_pointer_v<float>, std::is_pointer_v<float>);
    EXPECT_EQ(box::is_pointer_v<int*>, std::is_pointer_v<int*>);
    EXPECT_EQ(box::is_pointer_v<const int*>, std::is_pointer_v<const int*>);
    EXPECT_EQ(box::is_pointer_v<int* const>, std::is_pointer_v<int* const>);
    EXPECT_EQ(box::is_pointer_v<volatile int*>, std::is_pointer_v<volatile int*>);
    EXPECT_EQ(box::is_pointer_v<int* volatile>, std::is_pointer_v<int* volatile>);
    EXPECT_EQ(box::is_pointer_v<std::string>, std::is_pointer_v<std::string>);
}
