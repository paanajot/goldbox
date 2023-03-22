

#include "filter_optimizer.h"
#include "contours_extractor.h"
#include "corners_helper.h"

#include "utils/logger.h"

#include <limits>
#include <opencv2/imgproc.hpp>

namespace
{
constexpr int INITIAL_V_MIN = 125; // v works from 100 to 150
constexpr int INITIAL_S_MAX = 75; // s works from 5 to 155
const Boundary initial_lower = Boundary{0, 0, INITIAL_V_MIN};
const Boundary initial_upper = Boundary{179, INITIAL_S_MAX, 255};
const Boundaries initial_boundaries = Boundaries{initial_lower, initial_upper};

constexpr uint16_t S_MAX_ITERATIONS = 12;
constexpr uint16_t V_MAX_ITERATIONS = 4;
constexpr uint16_t MIN_FAILURE_TH = 3;
constexpr uint16_t MAX_FAILURE_TH = 250;
} // namespace

FilterOptimizer::Status
FilterOptimizer::validate_card_contours(const Image& image,
                                        const ContoursExtractor& contours_extractor)
{
    const auto& contours = contours_extractor.find_contours();
    const auto& contours_size = contours.size();

    if(contours_size < MIN_FAILURE_TH || contours_size > MAX_FAILURE_TH)
    {
        return FilterOptimizer::Status::FAILURE;
    }

    const auto& valid_contours = contours_extractor.filter_contours(contours);
    const auto& valid_corners_size =
        CornersHelper::generate_corners(image.size(), valid_contours).size();
    const auto& valid_contours_size = valid_contours.size();

    if(valid_contours_size < MIN_FAILURE_TH || valid_contours_size != valid_corners_size)
    {
        return FilterOptimizer::Status::FAILURE;
    }

    if(contours_size == valid_contours_size)
    {
        m_best_valid_contours_size = valid_contours_size;
        return FilterOptimizer::Status::SUCCESS;
    }

    const auto& first_condition = valid_contours_size > m_best_valid_contours_size;
    const auto& second_condition = valid_contours_size == m_best_valid_contours_size;
    const auto& third_condition = contours_size < m_best_contours_size;

    if(first_condition or (second_condition and third_condition))
    {
        m_best_valid_contours_size = valid_contours_size;
        m_best_contours_size = std::min(contours_size, m_best_contours_size);
        return FilterOptimizer::Status::SOLUTION_IMPROVED;
    }

    return FilterOptimizer::Status::FAILURE;
}

std::optional<Boundaries> FilterOptimizer::optimize(const Image& image)
{
    if(m_success_boundaries)
    {
        Logger::debug("Filter optimizer - success_boundaries");
        constexpr uint16_t SUCCESS_STEP = 1;
        const auto boundaries = optimize(image, *m_success_boundaries, SUCCESS_STEP, SUCCESS_STEP);
        if(boundaries)
        {
            return boundaries;
        }
    }

    if(m_improved_boundaries)
    {
        Logger::debug("Filter optimizer - improved_boundaries");
        constexpr uint16_t IMPROVED_STEP = 2;
        const auto boundaries =
            optimize(image, *m_improved_boundaries, IMPROVED_STEP, IMPROVED_STEP);
        if(boundaries)
        {
            return boundaries;
        }
    }

    Logger::debug("Filter optimizer - initial_boundaries");
    constexpr uint16_t INITIAL_STEP = 10;
    return optimize(image, initial_boundaries, INITIAL_STEP, INITIAL_STEP);
}

std::optional<Boundaries> FilterOptimizer::optimize(const Image& image,
                                                    const Boundaries& initial_boundaries,
                                                    uint16_t v_step,
                                                    uint16_t s_step)
{
    const ImageFilter image_filter{image};
    m_improved_boundaries = {};

    auto [lower, upper] = initial_boundaries;
    const auto initial_s_max = upper[1];

    // the bigger the better
    m_best_valid_contours_size = 0;

    // the lower the better
    m_best_contours_size = std::numeric_limits<size_t>::max();

    auto v_min = lower[2];
    for(uint16_t v = 1; v <= V_MAX_ITERATIONS; ++v)
    {
        lower[2] = v_min;
        auto s_max = initial_s_max;
        for(uint16_t s = 1; s <= S_MAX_ITERATIONS; ++s)
        {
            upper[1] = s_max;
            const auto& filtered_img = image_filter.filter(lower, upper);
            const ContoursExtractor contours_extractor{filtered_img};
            const auto& status = validate_card_contours(image, contours_extractor);

            if(status == FilterOptimizer::Status::SOLUTION_IMPROVED)
            {
                m_improved_boundaries = Boundaries{lower, upper};
            }
            else if(status == FilterOptimizer::Status::SUCCESS)
            {
                m_success_boundaries = Boundaries{lower, upper};
                return m_success_boundaries;
            }

            const auto s_sign = (s % 2) ? 1 : -1;
            s_max += s_sign * s * s_step;
        }
        const auto v_sign = (v % 2) ? 1 : -1;
        v_min += v_sign * v * v_step;
    }

    return m_improved_boundaries;
}

size_t FilterOptimizer::get_contours_size() const
{
    return m_best_valid_contours_size;
}
