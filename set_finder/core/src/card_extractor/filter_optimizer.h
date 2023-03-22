#pragma once

#include <optional>

#include "common/types.h"
#include "contours_extractor.h"
#include "image_filter.h"

#include "utils/logger.h"

class FilterOptimizer
{
public:
    std::optional<Boundaries> optimize(const Image& image);
    size_t get_contours_size() const;

private:
    enum class Status
    {
        SUCCESS = 0,
        SOLUTION_IMPROVED = 1,
        FAILURE = 2
    };

    size_t m_best_valid_contours_size{};
    size_t m_best_contours_size{};

    std::optional<Boundaries> m_success_boundaries{};
    std::optional<Boundaries> m_improved_boundaries{};

    std::optional<Boundaries> optimize(const Image& image,
                                       const Boundaries& initial_boundaries,
                                       uint16_t v_step,
                                       uint16_t s_step);
    FilterOptimizer::Status validate_card_contours(const Image& image,
                                                   const ContoursExtractor& contours_extractor);
};
