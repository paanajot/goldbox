
#include "detector/color_detector.h"

#include <fdeep/fdeep.hpp>

#include "model/fdeep_color_detector.h"
#include "utils/logger.h"

ColorDetector::ColorDetector()
{
    m_color_model = std::make_unique<fdeep::model>(
        fdeep::read_model_from_string(bin2cpp::getColorModel().getBuffer()));
}

ColorDetector::~ColorDetector() = default;

Feature ColorDetector::detect_feature(const CardPreprocessor& preprocessor) const
{
    const auto card_img = preprocessor.get_input_card();
    const auto input = fdeep::tensor_from_bytes(card_img.ptr(),
                                                static_cast<std::size_t>(card_img.rows),
                                                static_cast<std::size_t>(card_img.cols),
                                                static_cast<std::size_t>(card_img.channels()),
                                                0.0f,
                                                255.0f);

    return m_color_model->predict_class({input}) + 1;
}
