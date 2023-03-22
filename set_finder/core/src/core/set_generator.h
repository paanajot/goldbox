
#pragma once

#include <memory>
#include <vector>

#include "common/types.h"
#include "detector/detector.h"
#include "utils/logger.h"

class SetGenerator
{
public:
    SetGenerator();

    std::vector<Centroid>
    generate_set(const std::vector<std::pair<Centroid, Image>>& card_imgs) const;

    static std::vector<Centroid> update_set(const Image& image,
                                            const Contours& contours,
                                            const std::vector<Centroid>& set_points);

private:
    std::vector<Feature> detect_features(const CardPreprocessor& preprocessor) const;

    std::vector<std::unique_ptr<Detector>> m_detectors{};
};
