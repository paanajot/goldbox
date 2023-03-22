
#pragma once

#include "common/types.h"
#include <opencv2/core/types.hpp>

cv::Mat get_resized_image_keep_ratio(const cv::Mat& source_img, int width);

std::vector<cv::Point> transform_contour(const std::vector<cv::Point>& contour,
                                         const cv::Point2f& offset,
                                         float old_width,
                                         float new_width);

Centroid get_centroid(const Contour& contour);

// TODO: move to tests
void show_image(const cv::Mat& image);
