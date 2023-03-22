#pragma once

#include "common/types.h"

#include <vector>

class Extractor
{
public:
    std::vector<std::pair<Centroid, Image>> extract_cards(const Image& source_img,
                                                          const Boundaries& boundaries) const;
    Contours extract_contours(const Image& source_img, const Boundaries& boundaries) const;

private:
    Image get_warped_card(const Image& source_img, const Corners& corners) const;
};
