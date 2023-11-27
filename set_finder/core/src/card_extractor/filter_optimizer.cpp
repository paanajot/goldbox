

#include "filter_optimizer.h"
#include "contours_extractor.h"
#include "corners_helper.h"

#include "utils/hist_utils.h"
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

constexpr uint16_t NEGATIVE_PEAKS_TH = 100;
constexpr uint16_t S_MAX_ITERATIONS = 13;
constexpr uint16_t V_MAX_ITERATIONS = 7;
constexpr uint16_t MIN_FAILURE_TH = 1;
constexpr uint16_t MAX_FAILURE_TH = 250;

// Histogram parameters
constexpr int MAX_HIST_VALUE = 256;
const int hist_size[] = {256};
const float hranges[] = {0, static_cast<float>(MAX_HIST_VALUE)};
const float* ranges[] = {hranges};
const int vchannel{2}; // NOLINT
const int schannel{1};
const auto k_size = cv::Size(9, 9);
} // namespace

FilterOptimizer::Solution FilterOptimizer::generate_solution(const ImageFilter& image_filter,
                                                             const Boundaries& boundaries,
                                                             const uint32_t& best_cost) const
{
    Solution solution{};

    const auto& filtered_img = image_filter.filter(boundaries);
    const ContoursExtractor contours_extractor{filtered_img};

    const auto& contours = contours_extractor.find_contours();
    const auto& contours_size = contours.size();

    if(contours_size < MIN_FAILURE_TH || contours_size > MAX_FAILURE_TH)
    {
        return solution;
    }

    const auto& valid_contours = contours_extractor.filter_contours(contours);
    const auto& valid_corners_size =
        CornersHelper::generate_corners(filtered_img.size(), valid_contours).size();
    const auto& card_size = valid_contours.size();

    if(card_size < MIN_FAILURE_TH || card_size != valid_corners_size)
    {
        return solution;
    }

    const auto cost = (contours_size - card_size) + 100 * (20 - card_size);

    if(contours_size == card_size && cost < best_cost)
    {
        solution.boundaries = boundaries;
        solution.card_size = card_size;
        solution.cost = cost;
        solution.status = FilterOptimizer::Status::SUCCESS;
    }
    else if(cost < best_cost)
    {
        solution.boundaries = boundaries;
        solution.card_size = card_size;
        solution.cost = cost;
        solution.status = FilterOptimizer::Status::SOLUTION_IMPROVED;
    }

    return solution;
}

std::optional<Boundaries> FilterOptimizer::optimize(const Image& image)
{
    Logger::info("optimize!");
    const ImageFilter image_filter{image};

    if(m_solution)
    {
        const auto v_min_values = generate_v_min_values(image_filter);
        const auto s_max_values = generate_s_max_values(image_filter);
        const auto solution =
            optimize(image_filter, m_solution->boundaries, v_min_values, s_max_values);
        if(solution)
        {
            return solution->boundaries;
        }
    }

    const auto v_min_values = generate_v_min_values(image_filter);
    const auto s_max_values = generate_s_max_values(image_filter);
    const auto solution = optimize(image_filter, initial_boundaries, v_min_values, s_max_values);
    if(solution)
    {
        return solution->boundaries;
    }

    return {};
}

// NOLINTBEGIN [bugprone-easily-swappable-parameters]
std::optional<FilterOptimizer::Solution>
FilterOptimizer::optimize(const ImageFilter& image_filter,
                          const Boundaries& initial_boundaries,
                          std::vector<uint16_t> v_min_values,
                          std::vector<uint16_t> s_max_values)
// NOLINTEND
{
    std::optional<FilterOptimizer::Solution> best_solution{};

    auto [lower, upper] = initial_boundaries;

    for(const auto& v_min : v_min_values)
    {
        lower[2] = v_min;
        for(const auto& s_max : s_max_values)
        {
            upper[1] = s_max;
            const auto best_cost =
                (best_solution) ? best_solution->cost : std::numeric_limits<uint32_t>::max();

            const auto& solution = generate_solution(image_filter, {lower, upper}, best_cost);

            if(solution.status == FilterOptimizer::Status::SOLUTION_IMPROVED)
            {
                best_solution = solution;
            }
            else if(solution.status == FilterOptimizer::Status::SUCCESS)
            {
                Logger::debug("success!");
                m_solution = solution;
                return m_solution;
            }
        }
    }

    if((m_solution && best_solution && best_solution->cost <= m_solution->cost) ||
       (best_solution && !m_solution))
    {
        m_solution = best_solution;
    }
    else
    {
        m_solution = {};
    }

    return best_solution;
}

// NOLINTBEGIN [bugprone-easily-swappable-parameters]
std::vector<uint16_t> FilterOptimizer::generate_initial_values(uint16_t step,
                                                               uint16_t number_of_iterations,
                                                               uint16_t initial_value)
// NOLINTEND
{
    std::vector<uint16_t> values{};
    auto value = initial_value;
    for(uint16_t i = 0; i < number_of_iterations; ++i)
    {
        const auto sign = (i % 2) ? 1 : -1;
        value += sign * i * step;
        values.emplace_back(value);
    }

    return values;
}

std::vector<uint16_t>
FilterOptimizer::generate_initial_s_max_values(const ImageFilter& image_filter)
{
    // Histogram
    auto hsv_img = image_filter.get_hsv_img();
    cv::Mat histogram{};
    cv::calcHist(&hsv_img, 1, &schannel, {}, histogram, 1, hist_size, ranges);

    // smooth histogram
    cv::GaussianBlur(histogram, histogram, k_size, 0, 0, cv::BORDER_REPLICATE);

    const auto peaks = find_peaks(histogram, std::greater_equal<uint32_t>(), 10);
    auto negative_peaks = find_negative_peaks(histogram, NEGATIVE_PEAKS_TH);

    if(!peaks.empty() && !negative_peaks.empty())
    {
        const uint16_t initial_right_s = (negative_peaks.back() + peaks.back()) / 2;
        negative_peaks.emplace_back(initial_right_s);
    }

    m_s_step = std::ceil(static_cast<float>((negative_peaks.back() - negative_peaks.front())) /
                         static_cast<float>(negative_peaks.size()));
    return negative_peaks;
}

std::vector<uint16_t>
FilterOptimizer::generate_initial_v_min_values(const ImageFilter& image_filter)
{
    // Histogram
    auto hsv_img = image_filter.get_hsv_img();
    cv::Mat histogram{};
    cv::calcHist(&hsv_img, 1, &vchannel, {}, histogram, 1, hist_size, ranges);

    // smooth histogram
    cv::GaussianBlur(histogram, histogram, k_size, 0, 0, cv::BORDER_REPLICATE);

    const auto peaks = find_peaks(histogram, std::greater_equal<uint32_t>(), 10);
    auto negative_peaks = find_negative_peaks(histogram, NEGATIVE_PEAKS_TH);

    if(!peaks.empty() && !negative_peaks.empty())
    {
        const uint16_t initial_left_v = (negative_peaks.front() + peaks.front()) / 2;
        negative_peaks.insert(negative_peaks.cbegin(), initial_left_v);
    }

    m_v_step = std::ceil(static_cast<float>((negative_peaks.back() - negative_peaks.front())) /
                         static_cast<float>(negative_peaks.size()));

    return negative_peaks;
}

std::vector<uint16_t> FilterOptimizer::generate_s_max_values(const ImageFilter& image_filter)
{
    if(m_solution && m_s_step != 0)
    {
        const auto initial_s_max = m_solution->boundaries.second[1];
        const uint16_t s_adaptive_step =
            std::ceil(static_cast<double>(2 * m_s_step) / S_MAX_ITERATIONS);
        return generate_initial_values(s_adaptive_step, S_MAX_ITERATIONS, initial_s_max);
    }

    return generate_initial_s_max_values(image_filter);
}

std::vector<uint16_t> FilterOptimizer::generate_v_min_values(const ImageFilter& image_filter)
{
    if(m_solution && m_v_step != 0)
    {
        const auto v_min = m_solution->boundaries.first[2];
        const uint16_t v_step = std::ceil(static_cast<double>(2 * m_v_step) / V_MAX_ITERATIONS);
        return generate_initial_values(v_step, V_MAX_ITERATIONS, v_min);
    }

    return generate_initial_v_min_values(image_filter);
}

size_t FilterOptimizer::get_contours_size() const
{
    return (m_solution) ? m_solution->card_size : 0;
}
