#pragma once

#include "detector/card_preprocessor.h"
#include "detector/detector.h"

class ShapeDetector : public Detector
{
public:
    enum Shape
    {
        UNKNOWN = 0,
        DIAMOND = 1,
        OVAL = 2,
        SQUIGGLE = 3
    };

    Feature detect_feature(const CardPreprocessor& preprocessor) const override;
    ~ShapeDetector() override = default;
};
