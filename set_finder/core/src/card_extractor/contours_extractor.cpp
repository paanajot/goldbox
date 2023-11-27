
#include "contours_extractor.h"
#include "utils/logger.h"

#include <opencv2/imgproc.hpp>

namespace
{
constexpr auto CONTOUR_AREA_TH = 3000;
}

ContoursExtractor::ContoursExtractor(const Image& filtered_img)
    : m_filtered_img(filtered_img)
{ }

// TODO: move to algorithms
template <typename T>
T find_median(std::vector<T> v)
{
    const auto s = v.size();
    if(v.empty())
    {
        return {};
    }

    if(s == 1)
    {
        return v[0];
    }
    else if(s % 2 == 0)
    {
        std::nth_element(v.begin(), v.begin() + s / 2, v.end());
        std::nth_element(v.begin(), v.begin() + (s - 1) / 2, v.end());
        return static_cast<T>((v[(s - 1) / 2] + v[s / 2]) / 2.0); // NOLINT
    }
    else
    {
        std::nth_element(v.begin(), v.begin() + s / 2, v.end());
        return static_cast<T>(v[s / 2]);
    }
}

Contours ContoursExtractor::filter_contours(const Contours& contours) const
{
    Contours filtered_contours{};

    std::vector<int> areas{};
    areas.reserve(contours.size());

    const auto& th_filter = [&](const auto& contour) {
        const auto area = std::abs(cv::contourArea(contour));
        const auto is_above_th = (area > CONTOUR_AREA_TH);
        if(is_above_th)
            areas.emplace_back(area);

        return is_above_th;
    };
    std::copy_if(
        contours.cbegin(), contours.cend(), std::back_inserter(filtered_contours), th_filter);

    Contours valid_contours{};
    const auto area_median = find_median(areas);
    const auto& area_filter = [&](const auto& contour) {
        const auto area = std::abs(cv::contourArea(contour));
        return area > area_median * 0.4 && area < area_median * 1.6; // NOLINT
    };

    std::copy_if(filtered_contours.cbegin(),
                 filtered_contours.cend(),
                 std::back_inserter(valid_contours),
                 area_filter);

    return valid_contours;
}

Contours ContoursExtractor::find_contours() const
{
    constexpr int CANNY_TH1 = 30;
    constexpr int CANNY_TH2 = 200;

    Image gray_edged_img{};
    cv::cvtColor(m_filtered_img, gray_edged_img, cv::COLOR_BGR2GRAY);
    cv::Canny(gray_edged_img, gray_edged_img, CANNY_TH1, CANNY_TH2);

    Contours contours{};
    cv::findContours(gray_edged_img, contours, {}, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    return contours;
}
