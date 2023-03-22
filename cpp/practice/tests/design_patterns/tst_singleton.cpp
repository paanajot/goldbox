#include <gtest/gtest.h>

#include "singleton/singleton_data.h"

TEST(singleton, instance_test)
{
    const auto& first_reference = SingletonData::get_instance();
    const auto warszawa_population = first_reference.get_population("Warszawa");
    constexpr auto expected_population = 1'793'579u;
    EXPECT_EQ(expected_population, warszawa_population);

    constexpr auto beijing_population = 21'893'095u;
    auto& second_reference = SingletonData::get_instance();
    second_reference.add_city("Beijing", beijing_population);
    EXPECT_EQ(first_reference.get_population("Beijing"),
              second_reference.get_population("Beijing"));
}
