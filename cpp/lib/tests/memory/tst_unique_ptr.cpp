#include <gtest/gtest.h>
#include <utility>

#include "box/unique_ptr.h"

namespace
{
constexpr int value_1 = 10;
constexpr int value_2 = 21;

struct Foo
{
    Foo() = default;
    Foo(int x, double y)
        : m_x(x)
        , m_y(y)
    { }

    bool boo() const
    {
        return true;
    }

    int m_x{};
    double m_y{};
};

} // namespace

TEST(unique_ptr, move_constructor)
{
    box::unique_ptr ptr(new int(value_1));
    const auto ptr_2 = std::move(ptr);

    EXPECT_EQ(value_1, *ptr_2);
}

TEST(unique_ptr, move_operator)
{
    box::unique_ptr ptr(new int(value_1));
    box::unique_ptr ptr_2(new int(value_1));
    ptr_2 = std::move(ptr);

    EXPECT_EQ(value_1, *ptr_2);
}

TEST(unique_ptr, dereferences_pointer)
{
    box::unique_ptr ptr(new int(value_1));
    const box::unique_ptr ptr_2(new int(value_1));

    EXPECT_EQ(value_1, *ptr);
    EXPECT_EQ(value_1, *ptr_2);
}

TEST(unique_ptr, user_type_class)
{
    box::unique_ptr ptr(new Foo());

    EXPECT_TRUE(ptr->boo());
}

TEST(unique_ptr, release)
{
    box::unique_ptr ptr(new int(value_1));

    const auto* released_value = ptr.release();
    EXPECT_EQ(value_1, *released_value);
    EXPECT_EQ(nullptr, ptr.get());

    delete released_value;
}

TEST(unique_ptr, get)
{
    box::unique_ptr ptr(new int(value_1));

    EXPECT_EQ(value_1, *(ptr.get()));
}

TEST(unique_ptr, reset)
{
    box::unique_ptr ptr(new int(value_1));
    ptr.reset(new int(value_2));

    EXPECT_EQ(value_2, *(ptr.get()));
}

TEST(unique_ptr, bool_operator)
{
    box::unique_ptr ptr(new int(value_1));
    EXPECT_TRUE(ptr);

    int* nullptr_value = nullptr;
    box::unique_ptr ptr_2(nullptr_value);
    EXPECT_FALSE(ptr_2);
}

TEST(unique_ptr, make_unique)
{
    constexpr int val_1 = 1;
    constexpr double val_2 = 2.2;
    const auto ptr = box::make_unique<Foo>(val_1, val_2);

    EXPECT_TRUE(ptr);
    EXPECT_EQ(val_1, ptr->m_x);
    EXPECT_EQ(val_2, ptr->m_y);
}

TEST(unique_ptr, swap)
{
    auto ptr_1 = box::make_unique<int>(value_1);
    auto ptr_2 = box::make_unique<int>(value_2);

    EXPECT_EQ(value_1, *ptr_1);
    EXPECT_EQ(value_2, *ptr_2);

    ptr_1.swap(ptr_2);

    EXPECT_EQ(value_2, *ptr_1);
    EXPECT_EQ(value_1, *ptr_2);
}
