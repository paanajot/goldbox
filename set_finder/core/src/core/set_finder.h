#pragma once

#include "common/card.h"
#include "common/types.h"

#include <array>
#include <cstdint>
#include <optional>

class SetFinder
{
public:
    static constexpr uint8_t CARDS_IN_SET = 3;
    using SetIndices = std::array<uint8_t, CARDS_IN_SET>;

    static std::optional<SetIndices> find(const std::vector<Card>& cards);

private:
    static bool is_set(const std::vector<Card>& cards);
};
