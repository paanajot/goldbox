

#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "utils/algorithm.h"

cv::Mat get_resized_image_keep_ratio(const cv::Mat& source_img, int width)
{
    cv::Mat resized_img{};
    const auto source_height = static_cast<float>(source_img.size().height);
    const auto source_width = static_cast<float>(source_img.size().width);
    const auto height =
        static_cast<int>(source_height * (static_cast<float>(width) / source_width));
    cv::resize(source_img, resized_img, cv::Size{width, height}, 0, 0, cv::INTER_AREA);
    return resized_img;
}

std::vector<cv::Point> transform_contour(const std::vector<cv::Point>& contour,
                                         const cv::Point2f& offset,
                                         float old_width,
                                         float new_width)
{
    std::vector<cv::Point> resized_contour{};
    resized_contour.reserve(contour.size());

    const auto ratio = old_width / new_width;
    for(const auto& point : contour)
    {
        resized_contour.emplace_back(
            cv::Point{static_cast<int>(static_cast<float>(point.x) / ratio + offset.x),
                      static_cast<int>(static_cast<float>(point.y) / ratio + offset.y)});
    }
    return resized_contour;
}

Centroid get_centroid(const Contour& contour)
{
    const auto moments = cv::moments(contour);
    return {static_cast<int>(moments.m10 / moments.m00),
            static_cast<int>(moments.m01 / moments.m00)};
}

void show_image(const cv::Mat& image)
{
    cv::namedWindow("image", cv::WINDOW_AUTOSIZE);
    cv::imshow("image", image);
    while(true)
    {
        if((char)27 == (char)cv::waitKey(1)) // NOLINT
            break;
    }
}
