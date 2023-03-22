#pragma once

#include "common/types.h"
#include <vector>

class Card
{
public:
    cv::Point centroid;
    std::vector<Feature> features;
};
