#pragma once

#include <optional>

#include "common/types.h"
#include "contours_extractor.h"
#include "image_filter.h"

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

    struct Solution
    {
        Boundaries boundaries{};
        Status status = Status::FAILURE;
        uint32_t cost = std::numeric_limits<uint32_t>::max();
        size_t card_size = 0;
    };

    uint16_t m_v_step{};
    uint16_t m_s_step{};

    std::optional<Solution> m_solution;

    std::optional<FilterOptimizer::Solution> optimize(const ImageFilter& image_filter,
                                                      const Boundaries& initial_boundaries,
                                                      std::vector<uint16_t> v_min_values,
                                                      std::vector<uint16_t> s_max_values);

    FilterOptimizer::Solution generate_solution(const ImageFilter& image_filter,
                                                const Boundaries& boundaries,
                                                const uint32_t& best_cost) const;

    std::vector<uint16_t>
    generate_initial_values(uint16_t step, uint16_t number_of_iterations, uint16_t initial_value);

    std::vector<uint16_t> generate_initial_v_min_values(const ImageFilter& image_filter);
    std::vector<uint16_t> generate_v_min_values(const ImageFilter& image_filter);

    std::vector<uint16_t> generate_initial_s_max_values(const ImageFilter& image_filter);
    std::vector<uint16_t> generate_s_max_values(const ImageFilter& image_filter);
};
