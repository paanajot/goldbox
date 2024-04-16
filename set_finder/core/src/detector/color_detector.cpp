
#include "detector/color_detector.h"

#include "model/color_detector_model.h"
#include "utils/logger.h"

#include <algorithm>

#include <tensorflow/lite/interpreter.h>
#include <tensorflow/lite/model.h>

#include <tensorflow/lite/kernels/register.h>
#include <tensorflow/lite/optional_debug_tools.h>
#include <tensorflow/lite/string_util.h>

ColorDetector::ColorDetector()
{
    m_color_model = tflite::FlatBufferModel::BuildFromBuffer(
        bin2cpp::getColorModelFile().getBuffer(), bin2cpp::getColorModelFile().getSize());

    if(!m_color_model)
    {
        throw std::runtime_error("Failed to load TFLite model");
    }

    tflite::ops::builtin::BuiltinOpResolver op_resolver;
    tflite::InterpreterBuilder(*m_color_model, op_resolver)(&m_interpreter);

    if(m_interpreter->AllocateTensors() != kTfLiteOk)
    {
        throw std::runtime_error("Failed to allocate tensors");
    }
}

ColorDetector::~ColorDetector() = default;

Feature ColorDetector::detect_feature(const CardPreprocessor& preprocessor) const
{
    const auto card_img = preprocessor.get_input_card();

    Image img{};
    card_img.convertTo(img, CV_32FC4);

    memcpy(m_interpreter->typed_input_tensor<float>(0), img.data, img.total() * img.elemSize());

    if(m_interpreter->Invoke() != kTfLiteOk)
    {
        Logger::error("Invoke failed");
        return 0;
    }

    const auto* results = m_interpreter->typed_output_tensor<float>(0);
    const auto max_element_it = std::max_element(results, results + 3);
    return std::distance(results, max_element_it) + 1;
}
