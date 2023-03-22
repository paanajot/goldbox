
#include "detector/shading_detector.h"

#include <opencv2/imgproc.hpp>

#include <algorithm>
#include <functional>
#include <numeric>

#include "utils/logger.h"

namespace
{
// Histogram parameters
constexpr int MAX_HIST_VALUE = 256;
const int hist_size[] = {64};
const float hranges[] = {0, static_cast<float>(MAX_HIST_VALUE)};
const float* ranges[] = {hranges};
const int* channels{0}; // NOLINT

constexpr int DIAMETER = 7;
constexpr double SIGMA_COLOR = 40;
constexpr double SIGMA_SPACE = 40;
constexpr int CANNY_TH1 = 40;
constexpr int CANNY_TH2 = 120;

constexpr uint16_t SUM_RANGE = 2;

constexpr double OUTLINED_TH = 0.4; // works with 0.3 - 0.5
constexpr uint16_t HIST_TH = 100;
} // namespace

Feature ShadingDetector::detect_feature(const CardPreprocessor& preprocessor) const
{
    const auto contours = preprocessor.get_contours();
    if(contours.empty())
    {
        return Shading::UNKNOWN;
    }

    const auto gray_card = generate_gray_card(preprocessor);

    const auto bounding_rect = cv::boundingRect(contours[0]);
    const auto extended_rect = cv::Rect(bounding_rect.x - 1,
                                        bounding_rect.y - 1,
                                        bounding_rect.width + 2,
                                        bounding_rect.height + 2);

    if(extended_rect.x < 0 || extended_rect.y < 0 ||
       (extended_rect.x + extended_rect.width) >= gray_card.size().width ||
       (extended_rect.y + extended_rect.height) >= gray_card.size().height)
    {
        return Shading::UNKNOWN;
    }

    const auto roi = gray_card(bounding_rect);
    const auto extended_roi = gray_card(extended_rect);

    Image canny_card{};
    cv::Canny(extended_roi, canny_card, CANNY_TH1, CANNY_TH2);

    // TODO: filter simillar arc-length wise contours
    Contours roi_contours{};
    cv::findContours(canny_card, roi_contours, {}, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    cv::Mat histogram{};
    cv::calcHist(&roi, 1, channels, {}, histogram, 1, hist_size, ranges);

    const auto [left_x, right_x] =
        get_hist_range(histogram, 0, hist_size[0], std::greater_equal<uint16_t>());
    const auto [left_x_2, right_x_2] =
        get_hist_range(histogram, left_x, right_x, std::less_equal<uint16_t>());

    const auto is_valid = [&](const auto& x) {
        return 0 < (x - SUM_RANGE) && hist_size[0] > (x + SUM_RANGE);
    };

    if(!is_valid(left_x_2) || !is_valid(right_x_2))
    {
        return Shading::UNKNOWN;
    }

    const auto left_sum = std::accumulate(
        histogram.begin<float>(), histogram.begin<float>() + left_x_2 + SUM_RANGE, 0);

    const auto right_sum = std::accumulate(
        histogram.begin<float>() + right_x_2 - SUM_RANGE, histogram.end<float>(), 0);

    const auto contour_area = std::abs(cv::contourArea(contours[0]));
    const auto outer_pixels = bounding_rect.width * bounding_rect.height - contour_area;

    if(roi_contours.size() <= 2) // (left_sum >= 0.75 * contour_area) // works with 0.6 - 0.9
    {
        return Shading::SOLID;
    }
    else if((right_sum - outer_pixels) >= OUTLINED_TH * (abs(contour_area) - left_sum))
    {
        return Shading::OUTLINED;
    }

    return Shading::STRIPED;
}

Image ShadingDetector::generate_gray_card(const CardPreprocessor& preprocessor) const
{
    const auto card = preprocessor.get_filtered_card();

    Image bilateral_card{};
    cv::bilateralFilter(card, bilateral_card, DIAMETER, SIGMA_COLOR, SIGMA_SPACE);

    Image gray_card{};
    cv::cvtColor(bilateral_card, gray_card, cv::COLOR_BGR2GRAY);
    return gray_card;
}

template <typename Comparator>
std::pair<uint16_t, uint16_t> ShadingDetector::get_hist_range(cv::Mat1f histogram,
                                                              uint16_t begin,
                                                              uint16_t end,
                                                              Comparator comparator) const
{
    uint16_t left_x{}, right_x{};
    for(uint16_t i = begin; i < end; ++i)
    {
        const auto histogram_value = static_cast<uint32_t>(*histogram[i]);
        if(comparator(histogram_value, HIST_TH))
        {
            if(left_x == 0)
            {
                left_x = i;
            }
            right_x = i;
        }
    }

    return {left_x, right_x + 1};
}
