#include <gtest/gtest.h>

#include "common/card.h"
#include "core/set_finder.h"
#include "utils/logger.h"

using SetIndices = SetFinder::SetIndices;

struct TestData
{
    std::vector<Card> cards{};
    std::optional<SetIndices> set_indices{};
};

::std::ostream& operator<<(::std::ostream& os, const TestData& test_data)
{
    os << "number_of_cards_" << test_data.cards.size();
    if(test_data.set_indices)
    {
        const auto set_indices = *test_data.set_indices;
        os << "_set_indices";
        for(const auto index : set_indices)
        {
            os << '_' << static_cast<uint32_t>(index);
        }
    }

    return os;
}

const Card card_1{{}, {1, 1, 1, 1}};
const Card card_2{{}, {2, 2, 2, 2}};
const Card card_3{{}, {3, 3, 3, 3}};

const Card card_4{{}, {1, 1, 2, 2}};
const Card card_5{{}, {1, 2, 1, 2}};
const Card card_6{{}, {3, 1, 2, 2}};

const TestData data_1{{card_1, card_2, card_3}, SetIndices{0, 1, 2}};
const TestData data_2{{card_1, card_2, card_3, card_4}, SetIndices{0, 1, 2}};
const TestData data_3{{card_4, card_1, card_2, card_3}, SetIndices{1, 2, 3}};
const TestData data_4{{card_4, card_2, card_3}, {}};
const TestData data_5{{card_4, card_5, card_6, card_1, card_2, card_3}, SetIndices{3, 4, 5}};
const TestData data_6{{card_4, card_1, card_5, card_2, card_6, card_3}, SetIndices{1, 3, 5}};
const TestData data_7{{}, {}};
const TestData data_8{{card_1}, {}};
const TestData data_9{{card_3, card_2, card_1}, SetIndices{0, 1, 2}};

const auto input_values =
    testing::Values(data_1, data_2, data_3, data_4, data_5, data_6, data_7, data_8, data_9);

class SetFinderTest : public testing::TestWithParam<TestData>
{ };

TEST_P(SetFinderTest, validate_if_set_is_found_correctly)
{
    Logger::enable_color();
    const auto params = GetParam();
    const auto cards = params.cards;
    const auto set_indices = params.set_indices;

    const auto actual_set_indices = SetFinder::find(cards);
    EXPECT_EQ(set_indices, actual_set_indices);
}

INSTANTIATE_TEST_SUITE_P(SetFinderTestSuite, SetFinderTest, input_values);
