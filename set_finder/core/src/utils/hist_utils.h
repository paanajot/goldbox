
#pragma once

#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "common/types.h"

// NOLINTBEGIN [readability-magic-numbers]
template <typename Comparator>
std::pair<uint16_t, uint16_t> get_hist_range(cv::Mat1i histogram,
                                             Comparator comparator,
                                             uint16_t begin = 0,
                                             uint16_t end = 256,
                                             uint16_t hist_th = 100);

template <typename Comparator>
std::vector<uint16_t>
find_peaks(cv::Mat1i histogram, Comparator comparator, uint16_t hist_th = 100);

inline void draw_histogram(cv::Mat histogram, bool wait_for_key = false, uint16_t hist_size = 256);

inline std::vector<uint16_t> find_negative_peaks(cv::Mat1i histogram, uint16_t hist_th)
{
    constexpr uint16_t HIST_SIZE = 256;
    std::vector<uint16_t> peaks{};
    int last_positive_peak{};
    for(uint16_t i = 1; i < HIST_SIZE - 1; ++i)
    {
        const auto center_h = *histogram[i];
        const auto left_h = *histogram[i - 1];
        const auto right_h = *histogram[i + 1];

        last_positive_peak = std::max(last_positive_peak, center_h);
        const auto diff_height = last_positive_peak - center_h;
        if(center_h <= left_h && center_h <= right_h && left_h != right_h && diff_height > hist_th)
        {
            last_positive_peak = 0;
            peaks.emplace_back(i);
        }
    }

    return peaks;
}

// NOLINTEND

/******************************************************/
/*                          IMPL                      */
/******************************************************/

template <typename Comparator>
std::pair<uint16_t, uint16_t> get_hist_range(
    cv::Mat1i histogram, Comparator comparator, uint16_t begin, uint16_t end, uint16_t hist_th)
{
    uint16_t left_x{}, right_x{};
    for(uint16_t i = begin; i < end; ++i)
    {
        const auto histogram_value = static_cast<uint32_t>(*histogram[i]);
        if(comparator(histogram_value, hist_th))
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

template <typename Comparator>
std::vector<uint16_t> find_peaks(cv::Mat1i histogram, Comparator comparator, uint16_t hist_th)
{
    constexpr uint16_t HIST_SIZE = 256;
    std::vector<uint16_t> peaks{};
    for(uint16_t i = 1; i < HIST_SIZE - 1; ++i)
    {
        const auto center_h = *histogram[i];
        const auto left_h = *histogram[i - 1];
        const auto right_h = *histogram[i + 1];
        if(comparator(center_h, left_h) && comparator(center_h, right_h) && left_h != right_h &&
           comparator(center_h, hist_th))
        {
            peaks.emplace_back(i);
        }
    }

    return peaks;
}

void draw_histogram(cv::Mat histogram, bool wait_for_key, uint16_t hist_size)
{
    constexpr int HIST_W = 512;
    constexpr int HIST_H = 400;
    int bin_w = cvRound(static_cast<double>(HIST_W) / hist_size);

    Image hist_image(HIST_H, HIST_W, CV_8UC3, cv::Scalar(0, 0, 0));

    const auto blue_color = cv::Scalar(255, 0, 0);
    constexpr int THICKNESS = 2;

    cv::normalize(histogram, histogram, 0, hist_image.rows, cv::NORM_MINMAX, -1, cv::Mat());
    for(uint16_t i = 1; i < hist_size; i++)
    {
        cv::line(hist_image,
                 cv::Point(bin_w * (i - 1), HIST_H - cvRound(histogram.at<float>(i - 1))),
                 cv::Point(bin_w * (i), HIST_H - cvRound(histogram.at<float>(i))),
                 blue_color,
                 THICKNESS);
    }

    cv::namedWindow("histogram", cv::WINDOW_AUTOSIZE);
    cv::imshow("histogram", hist_image);

    if(wait_for_key)
    {
        while(true)
        {
            if((char)27 == (char)cv::waitKey(1)) // NOLINT
                break;
        }
    }
}
