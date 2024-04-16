#pragma once

#include "detector/card_preprocessor.h"
#include "detector/detector.h"

#include <memory>

namespace tflite
{
class FlatBufferModel;
class Interpreter;
} // namespace tflite

class ColorDetector : public Detector
{
public:
    ColorDetector();
    ~ColorDetector() override;

    Feature detect_feature(const CardPreprocessor& preprocessor) const override;

private:
    std::unique_ptr<tflite::FlatBufferModel> m_color_model;
    std::unique_ptr<tflite::Interpreter> m_interpreter;
};
