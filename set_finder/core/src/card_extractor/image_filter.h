#pragma once

#include "common/types.h"

class ImageFilter
{
public:
    explicit ImageFilter(const Image& source_img);

    Image filter(const Boundary& lower, const Boundary& upper) const;

    // TODO: use it to calculate initial s min
    int get_s_mean() const;

private:
    Image m_blured_img{};
    Image m_hsv_img{};

    int m_s_mean{};
};
