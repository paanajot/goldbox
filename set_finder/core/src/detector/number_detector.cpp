
#include "detector/number_detector.h"

Feature NumberDetector::detect_feature(const CardPreprocessor& preprocessor) const
{
    const auto& number = preprocessor.get_contours().size();
    return (number <= 3) ? number : 0;
}
