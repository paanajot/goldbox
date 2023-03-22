
#include "detector/shape_detector.h"
#include "utils/logger.h"

#include <algorithm>
#include <opencv2/imgproc.hpp>

namespace
{
constexpr uint32_t DIAMOND_DIFF_AREA_TH = 1800;
constexpr uint32_t OVAL_DIFF_AREA_TH = 1000;
constexpr uint32_t OVAL_AREA_TH = 3600;
} // namespace

Feature ShapeDetector::detect_feature(const CardPreprocessor& preprocessor) const
{
    const auto& contours = preprocessor.get_contours();
    if(contours.empty())
    {
        return Shape::UNKNOWN;
    }

    const auto biggest_contour = std::max_element(
        std::begin(contours), std::end(contours), [](const auto& contour1, const auto& contour2) {
            return std::abs(cv::contourArea(contour1)) > std::abs(cv::contourArea(contour2));
        });

    const auto biggest_area = abs(cv::contourArea(*biggest_contour, true));
    const auto bounding_rect = cv::boundingRect(*biggest_contour);
    const auto diff_area = bounding_rect.width * bounding_rect.height - biggest_area;

    if(diff_area >= DIAMOND_DIFF_AREA_TH)
    {
        return Shape::DIAMOND;
    }
    else if(biggest_area >= OVAL_AREA_TH and diff_area <= OVAL_DIFF_AREA_TH)
    {
        return Shape::OVAL;
    }

    return Shape::SQUIGGLE;
}
