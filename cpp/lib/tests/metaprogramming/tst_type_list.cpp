#include <gtest/gtest.h>
#include <string>

#include "box/type_list.h"

TEST(type_list, front)
{
    using SimpleTypeList = box::TypeList<int, std::string, double, float>;
    EXPECT_EQ(typeid(int), typeid(box::Front<SimpleTypeList>));
}

TEST(type_list, back)
{
    using SimpleTypeList = box::TypeList<int, std::string, double, float>;
    EXPECT_EQ(typeid(float), typeid(box::Back<SimpleTypeList>));
}

TEST(type_list, pop_front)
{
    using SimpleTypeList = box::TypeList<int, std::string, double, float>;
    EXPECT_EQ(typeid(std::string), typeid(box::Front<box::PopFront<SimpleTypeList>>));
}

TEST(type_list, push_back)
{
    using SimpleTypeList = box::TypeList<int, std::string, double, float>;
    EXPECT_EQ(typeid(char), typeid(box::Back<box::PushBack<SimpleTypeList, char>>));
}

TEST(type_list, reverse_list)
{
    using SimpleTypeList = box::TypeList<int, std::string, double, float>;
    using MyReversedList = box::ReverseList<SimpleTypeList>;

    using ExpectResult = box::TypeList<float, double, std::string, int>;

    EXPECT_EQ(typeid(ExpectResult), typeid(MyReversedList));
}

TEST(type_list, is_empty)
{
    using SimpleTypeList = box::TypeList<int, std::string, double, float>;

    EXPECT_TRUE(box::is_empty<box::TypeList<>>);
    EXPECT_FALSE(box::is_empty<SimpleTypeList>);
}

TEST(type_list, nth_element)
{
    using SimpleTypeList = box::TypeList<int, std::string, double, float>;

    using Nth0 = box::NthElement<SimpleTypeList, 0>;
    using Nth1 = box::NthElement<SimpleTypeList, 1>;
    using Nth2 = box::NthElement<SimpleTypeList, 2>;
    using Nth3 = box::NthElement<SimpleTypeList, 3>;

    EXPECT_EQ(typeid(int), typeid(Nth0));
    EXPECT_EQ(typeid(std::string), typeid(Nth1));
    EXPECT_EQ(typeid(double), typeid(Nth2));
    EXPECT_EQ(typeid(float), typeid(Nth3));

    EXPECT_NE(typeid(int), typeid(Nth3));
}

TEST(type_list, largest_type)
{
    using MyLargestType1 = box::LargestType<box::TypeList<char, int, double, long double>>;
    EXPECT_EQ(sizeof(long double), sizeof(MyLargestType1));

    using MyLargestType2 = box::LargestType<box::TypeList<char, int>>;
    EXPECT_EQ(sizeof(int), sizeof(MyLargestType2));

    using MyLargestType3 = box::LargestType<box::TypeList<char, int, std::string>>;
    EXPECT_EQ(sizeof(std::string), sizeof(MyLargestType3));

    using MyLargestType4 = box::LargestType<box::TypeList<char, long double>>;
    EXPECT_NE(sizeof(char), sizeof(MyLargestType4));
}
