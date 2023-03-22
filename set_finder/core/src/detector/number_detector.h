#pragma once

#include "detector/card_preprocessor.h"
#include "detector/detector.h"

class NumberDetector : public Detector
{
public:
    Feature detect_feature(const CardPreprocessor& preprocessor) const override;
    ~NumberDetector() override = default;
};
