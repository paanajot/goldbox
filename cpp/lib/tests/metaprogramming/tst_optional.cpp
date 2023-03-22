#include <compare>
#include <complex>
#include <cstdint>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>

#include "box/optional.h"
#include <gtest/gtest.h>

namespace
{
class UserName
{
    std::string m_name{};

public:
    UserName()
        : m_name("Default")
    { }
};

struct Person
{
    std::string m_name{};
    uint32_t m_age{};

    Person(const std::string& name, uint32_t age)
        : m_name(name)
        , m_age(age)
    { }
};

struct Point
{
    constexpr Point(const int32_t& a, const int32_t& b)
        : m_x(a)
        , m_y(b)
    { }

    const int32_t& m_x;
    const int32_t& m_y;
};

struct OneBit
{
    bool m_flag : 1;
};

constexpr uint32_t buffer_size = 15;
int buffer[buffer_size];
auto boo()
{
    return box::make_optional(buffer);
}

void foo(int* = nullptr) { }

auto buzz()
{
    return box::make_optional(foo);
}

constexpr int32_t int_val_1 = 1;
constexpr int32_t int_val_2 = 2;
constexpr double double_val_1 = -1.2;
constexpr double double_val_2 = 21.2;
} // namespace

TEST(optional, empty)
{
    [[maybe_unused]] box::optional<int32_t> empty;
    [[maybe_unused]] box::optional<float> o_float = box::nullopt;
}

TEST(optional, direct)
{
    [[maybe_unused]] box::optional<int32_t> o_int_1(int_val_1);
    [[maybe_unused]] box::optional<int32_t> o_int_2{int_val_2};
    [[maybe_unused]] box::optional int_deduced(int_val_1); // deduction guides
}

TEST(optional, in_place)
{
    // NOTE: That works but it creates an additional temporary object.
    [[maybe_unused]] box::optional<UserName> u1{UserName()};

    // Here we use a more efficient constructor.
    [[maybe_unused]] box::optional<UserName> u2{box::in_place};
    [[maybe_unused]] box::optional<std::complex<double>> c{
        box::in_place, double_val_1, double_val_2};
}

TEST(optional, copy_constructor)
{
    box::optional<int32_t> o_int_1(int_val_1);
    box::optional<int32_t> o_int_2(o_int_1);
    EXPECT_EQ(int_val_1, *o_int_2);
}

TEST(optional, converting_copy_constructor)
{
    box::optional<int32_t> o_int(int_val_1);
    box::optional<double> o_double(o_int);
    EXPECT_EQ(int_val_1, *o_double);
}

TEST(optional, move_constructor)
{
    box::optional<int32_t> o_int_1(int_val_1);
    box::optional<int32_t> o_int_2 = std::move(o_int_1);
    EXPECT_EQ(int_val_1, *o_int_2);
}

TEST(optional, initializer_list_constructor)
{
    constexpr uint32_t john_age = 19;
    box::optional<Person> op(box::in_place, {'J', 'o', 'h', 'n'}, john_age);
    EXPECT_EQ(john_age, op->m_age);
    EXPECT_EQ("John", op->m_name);
}

TEST(optional, perfect_forwarding)
{
    int32_t x = int_val_1;
    int32_t y = int_val_2;
    box::optional<Point> p{box::in_place, x, y};
    x = int_val_2;

    // NOTE: This works thanks to the perfect forwarding feature.
    EXPECT_EQ(x, p->m_x);
}

TEST(optional, data_access)
{
    box::optional<Point> p{box::in_place, int_val_1, int_val_2};
    EXPECT_EQ(int_val_1, p->m_x);
    EXPECT_EQ(int_val_2, p->m_y);

    box::optional<int32_t> o_int{int_val_1};
    EXPECT_EQ(int_val_1, *o_int);

    EXPECT_EQ(int_val_1, *std::move(o_int));
}

TEST(optional, const_data_access)
{
    const box::optional<Point> p{box::in_place, int_val_1, int_val_2};
    EXPECT_EQ(int_val_1, p->m_x);
    EXPECT_EQ(int_val_2, p->m_y);

    const box::optional<int32_t> o_int{int_val_1};
    EXPECT_EQ(int_val_1, *o_int);

    EXPECT_EQ(int_val_1, *std::move(o_int));
}

TEST(optional, constexpr_data_access)
{
    constexpr box::optional<Point> p{box::in_place, int_val_1, int_val_2};
    EXPECT_EQ(int_val_1, p->m_x);
    EXPECT_EQ(int_val_2, p->m_y);

    constexpr box::optional<int32_t> o_int{int_val_1};
    EXPECT_EQ(int_val_1, *o_int);

    EXPECT_EQ(int_val_1, *std::move(o_int));
}

TEST(optional, checks_whether_the_object_contains_a_value)
{
    box::optional<int32_t> op{int_val_1};
    EXPECT_TRUE(op.has_value());
    EXPECT_TRUE(op);

    box::optional<int32_t> op2{};
    EXPECT_FALSE(op2.has_value());
    EXPECT_FALSE(op2);
}

TEST(optional, make_optional_value)
{
    std::string str{"hello world"};
    const std::string expected_str{str};
    const auto op = box::make_optional(std::move(str));

    EXPECT_EQ(expected_str, *op);
}

TEST(optional, decay)
{
    // NOTE: if decay_t is not used in make_optional following three lines don't compile
    foo(*boo());
    buzz();

    auto op = box::make_optional<const int32_t>(std::move(int_val_1));
    *op = int_val_2;
    EXPECT_EQ(int_val_2, *op);
}

TEST(optional, make_optional_args)
{
    const auto op = box::make_optional<Point>(int_val_1, int_val_2);
    EXPECT_EQ(int_val_1, op->m_x);
    EXPECT_EQ(int_val_2, op->m_y);
}

TEST(optional, make_optional_initializer_list)
{
    constexpr uint32_t john_age = 19;
    const auto op = box::make_optional<Person>({'J', 'o', 'h', 'n'}, john_age);
    EXPECT_EQ(john_age, op->m_age);
    EXPECT_EQ("John", op->m_name);
}

TEST(optional, value)
{
    box::optional<int32_t> op(int_val_1);
    EXPECT_EQ(int_val_1, op.value());

    box::optional<int32_t> op_2{};
    EXPECT_THROW(op_2.value(), box::bad_optional_access);
}

TEST(optional, const_value)
{
    const box::optional<int32_t> op(int_val_1);
    EXPECT_EQ(int_val_1, op.value());

    const box::optional<int32_t> op_2{};
    EXPECT_THROW(op_2.value(), box::bad_optional_access);
}

TEST(optional, value_or)
{
    box::optional<int32_t> op(int_val_1);
    EXPECT_EQ(int_val_1, op.value_or(int_val_2));

    box::optional<int32_t> op_2{};
    EXPECT_EQ(int_val_2, op_2.value_or(int_val_2));
}

TEST(optional, rvalue_or)
{
    EXPECT_EQ(int_val_1, box::optional<int32_t>(int_val_1).value_or(int_val_2));
    EXPECT_EQ(int_val_2, box::optional<int32_t>().value_or(int_val_2));
}

TEST(optional, rvalue)
{
    EXPECT_EQ(int_val_1, box::optional<int32_t>(int_val_1).value());
    EXPECT_THROW(box::optional<int32_t>().value(), box::bad_optional_access);
}

TEST(optional, const_rvalue)
{
    const box::optional<int32_t> op(int_val_1);
    EXPECT_EQ(int_val_1, std::move(op).value());

    const box::optional<int32_t> op_2{};
    EXPECT_THROW(std::move(op_2).value(), box::bad_optional_access);
}

TEST(optional, sizeof_comparison)
{
    const box::optional<int32_t> box_op(int_val_1);
    const std::optional<int32_t> std_op(int_val_1);
    EXPECT_EQ(sizeof(box_op), sizeof(std_op));

    const box::optional<uint16_t> box_op_2(int_val_1);
    const std::optional<uint16_t> std_op_2(int_val_1);
    EXPECT_EQ(sizeof(box_op_2), sizeof(std_op_2));

    const box::optional<OneBit> box_op_3{};
    const std::optional<OneBit> std_op_3{};
    EXPECT_EQ(sizeof(box_op_3), sizeof(std_op_3));
}

TEST(optional, three_way_comparison)
{
    const box::optional<int32_t> op_1(int_val_1);
    const box::optional<int32_t> op_2(int_val_1);
    const box::optional<int32_t> op_3(int_val_2);

    EXPECT_TRUE(op_1 == op_2);
    EXPECT_TRUE(op_1 != box::nullopt);
    EXPECT_TRUE(op_1 <= op_3);
    EXPECT_TRUE(op_3 >= op_1);

    EXPECT_TRUE(box::optional<int32_t>() == box::nullopt);
}
