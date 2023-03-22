#include <functional> // NOLINT
#include <future>
#include <gtest/gtest.h>
#include <system_error>

#include "shared_mutex.h"

TEST(concurrency, shared_mutex)
{
    using namespace std::placeholders;

    PhoneBook phone_book{};
    const auto writer = std::bind(&PhoneBook::add_number, &phone_book, _1, _2);
    const auto reader = std::bind(&PhoneBook::read_number, &phone_book, _1);

    constexpr PhoneBook::Number hans_number = 123456789;
    constexpr PhoneBook::Number joanna_number = 644233456;

    auto f1 = std::async(writer, "Hans", hans_number);
    auto f3 = std::async(writer, "Joanna", joanna_number);
    EXPECT_NO_THROW(f1.get());
    EXPECT_NO_THROW(f3.get());

    auto f2 = std::async(reader, "Hans");
    auto f4 = std::async(reader, "Joanna");
    auto f5 = std::async(reader, "Hans");
    EXPECT_EQ(f2.get(), hans_number);
    EXPECT_EQ(f4.get(), joanna_number);
    EXPECT_EQ(f5.get(), hans_number);
}
