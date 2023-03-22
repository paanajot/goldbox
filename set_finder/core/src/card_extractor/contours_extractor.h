#pragma once

#include <utility>
#include <vector>

#include "common/types.h"

class ContoursExtractor
{
public:
    explicit ContoursExtractor(const Image& filtered_img);

    Contours find_contours() const;
    Contours filter_contours(const Contours& contours) const;

private:
    Image m_filtered_img;
};
