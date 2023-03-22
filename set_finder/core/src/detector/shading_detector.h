#pragma once

#include "detector/card_preprocessor.h"
#include "detector/detector.h"

#include <utility>

class ShadingDetector : public Detector
{
public:
    enum Shading
    {
        UNKNOWN = 0,
        SOLID = 1,
        OUTLINED = 2,
        STRIPED = 3
    };

    Feature detect_feature(const CardPreprocessor& preprocessor) const override;
    ~ShadingDetector() override = default;

private:
    Image generate_gray_card(const CardPreprocessor& preprocessor) const;

    template <typename Comparator>
    std::pair<uint16_t, uint16_t>
    get_hist_range(cv::Mat1f histogram, uint16_t begin, uint16_t end, Comparator comparator) const;
};
