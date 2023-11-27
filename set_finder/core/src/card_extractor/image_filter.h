#pragma once

#include "common/types.h"

class ImageFilter
{
public:
    explicit ImageFilter(const Image& source_img);

    Image filter(const Boundary& lower, const Boundary& upper) const;
    Image filter(const Boundaries& boundaries) const;

    // TODO: use it to calculate initial s min
    int get_s_mean() const;
    int get_v_mean() const;

    Image get_hsv_img() const;

    std::pair<uint16_t, uint16_t> get_v_peaks() const;

private:
    Image m_blured_img{};
    Image m_hsv_img{};

    int m_s_mean{};
    int m_v_mean{};
};
