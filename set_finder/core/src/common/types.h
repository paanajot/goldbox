
#pragma once

#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>

#include <cstdint>
#include <vector>

using Image = cv::Mat;
using Centroid = cv::Point2i;

using Boundary = cv::Vec3i;
using Boundaries = std::pair<Boundary, Boundary>;

using Contour = std::vector<cv::Point>;
using Contours = std::vector<Contour>;

using Corners = std::vector<cv::Point>;
using Corners2F = std::vector<cv::Point2f>;
using ImageSize = cv::Size2i;

using Feature = uint8_t;
