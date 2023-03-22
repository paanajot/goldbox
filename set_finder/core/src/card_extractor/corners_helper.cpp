
#include "corners_helper.h"

#include "common/types.h"

#include <algorithm>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

namespace
{
constexpr int CONTOUR_IDX = -1;
constexpr int THICKNESS = 2;
constexpr int WHITE_COLOR = 255;

constexpr int MAX_CORNERS = 4;
constexpr double QUALITY_LEVEL = 0.01;
constexpr int MIN_DISTANCE = 50;
} // namespace

Corners2F CornersHelper::order_corners(const Corners& corners)
{
    // TODO: use ranges
    const auto& point_sum = [&](const auto& lhs, const auto& rhs) {
        return (lhs.x + lhs.y) < (rhs.x + rhs.y);
    };
    const auto& [min_sum, max_sum] =
        std::minmax_element(corners.cbegin(), corners.cend(), point_sum);

    const auto& point_diff = [](const auto& lhs, const auto& rhs) {
        return (lhs.x - lhs.y) > (rhs.x - rhs.y);
    };
    const auto& [min_diff, max_diff] =
        std::minmax_element(corners.cbegin(), corners.cend(), point_diff);

    return {*min_sum, *min_diff, *max_sum, *max_diff};
}

std::vector<Corners> CornersHelper::generate_corners(const ImageSize& image_size,
                                                     const Contours& contours)
{
    std::vector<Corners> all_corners{};
    for(const auto& contour : contours)
    {
        Contour convex_hull{};
        cv::convexHull(contour, convex_hull);

        Image hull_img = cv::Mat::zeros(image_size, CV_8U);
        cv::drawContours(hull_img, Contours{convex_hull}, CONTOUR_IDX, WHITE_COLOR, THICKNESS);

        Corners corners{};
        cv::goodFeaturesToTrack(hull_img, corners, MAX_CORNERS, QUALITY_LEVEL, MIN_DISTANCE);

        if(corners.size() != 4)
            return {};

        all_corners.emplace_back(corners);
    }

    return all_corners;
}
