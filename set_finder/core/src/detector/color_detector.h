#pragma once

#include "detector/card_preprocessor.h"
#include "detector/detector.h"

#include <memory>

namespace fdeep
{
class model;
}

class ColorDetector : public Detector
{
public:
    ColorDetector();
    ~ColorDetector() override;

    Feature detect_feature(const CardPreprocessor& preprocessor) const override;

private:
    std::unique_ptr<fdeep::model> m_color_model;
};
