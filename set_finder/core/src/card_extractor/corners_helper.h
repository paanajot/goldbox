#pragma once

#include "common/types.h"

#include <vector>

class CornersHelper
{
public:
    static Corners2F order_corners(const Corners& corners);
    static std::vector<Corners> generate_corners(const ImageSize& image_size,
                                                 const Contours& contours);
};
