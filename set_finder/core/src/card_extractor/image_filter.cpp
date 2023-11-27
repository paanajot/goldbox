
#include "image_filter.h"

#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>

#include "utils/logger.h"

namespace
{
constexpr auto SIGMA_X = 0;
}

ImageFilter::ImageFilter(const Image& source_img)
{
    const auto kernel = cv::Size(7, 7);

    // It works faster with blur
    cv::GaussianBlur(source_img, m_blured_img, kernel, SIGMA_X);
    cv::cvtColor(m_blured_img, m_hsv_img, cv::COLOR_BGR2HSV);

    const auto nopixels = static_cast<int>(m_hsv_img.size().area() * 0.20);

    std::vector<cv::Mat> channels(3);
    cv::split(m_hsv_img, channels);
    auto s_channel = channels.at(1);
    auto v_channel = channels.at(2);

    const auto flatten_s_channel = s_channel.reshape(1, 1);
    cv::Mat s_sorted{};
    cv::sort(flatten_s_channel, s_sorted, cv::SORT_ASCENDING | cv::SORT_EVERY_ROW);
    m_s_mean = static_cast<int>(cv::mean(s_sorted.colRange(0, nopixels))[0]);

    const auto flatten_v_channel = v_channel.reshape(1, 1);
    cv::Mat v_sorted{};
    cv::sort(flatten_v_channel, v_sorted, cv::SORT_DESCENDING | cv::SORT_EVERY_ROW);
    m_v_mean = static_cast<int>(cv::mean(v_sorted.colRange(0, nopixels))[0]);
}

Image ImageFilter::filter(const Boundary& lower, const Boundary& upper) const
{
    Image mask{}, filtered_img{};

    cv::inRange(m_hsv_img, lower, upper, mask);
    cv::bitwise_and(m_blured_img, m_blured_img, filtered_img, mask);

    return filtered_img;
}

Image ImageFilter::filter(const Boundaries& boundaries) const
{
    return filter(boundaries.first, boundaries.second);
}

Image ImageFilter::get_hsv_img() const
{
    return m_hsv_img;
}

int ImageFilter::get_s_mean() const
{
    return m_s_mean;
}

int ImageFilter::get_v_mean() const
{
    return m_v_mean;
}
