
#include "core/set_finder.h"

#include "utils/combination.h"

#include <algorithm>
#include <array>

std::optional<SetFinder::SetIndices> SetFinder::find(const std::vector<Card>& cards)
{
    const uint32_t cards_size = cards.size();
    if(cards_size < CARDS_IN_SET)
    {
        return {};
    }

    utils::Combination<CARDS_IN_SET> combination{cards_size};

    while(combination.is_valid())
    {
        std::vector<Card> set_cards{};
        set_cards.reserve(CARDS_IN_SET);
        const auto indices = combination.next();
        for(const auto& idx : indices)
        {
            set_cards.emplace_back(cards.at(idx));
        }

        if(is_set(set_cards))
        {
            return indices;
        }
    };

    return {};
}

bool SetFinder::is_set(const std::vector<Card>& cards)
{
    std::array<uint8_t, 4> features{};
    for(const auto& card : cards)
    {
        std::transform(features.begin(),
                       features.end(),
                       card.features.begin(),
                       features.begin(),
                       std::plus<uint8_t>());
    }
    return std::all_of(
        features.cbegin(), features.cend(), [](const auto& f) { return f % 3 == 0; });
}
