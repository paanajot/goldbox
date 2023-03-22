
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

Contours ContoursExtractor::filter_contours(const Contours& contours) const
{
    Contours valid_contours{};

    const auto& filter = [&](const auto& contour) {
        return (std::abs(cv::contourArea(contour)) > CONTOUR_AREA_TH);
    };
    std::copy_if(contours.cbegin(), contours.cend(), std::back_inserter(valid_contours), filter);

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
