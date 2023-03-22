#pragma once

#include "detector/card_preprocessor.h"

#include <cstdint>

class Detector
{
public:
    virtual Feature detect_feature(const CardPreprocessor& preprocessor) const = 0;
    virtual ~Detector() = default;
};
