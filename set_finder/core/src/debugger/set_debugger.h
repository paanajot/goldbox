
#pragma once

#include <atomic>
#include <memory>
#include <optional>
#include <vector>

#include "common/types.h"
#include "detector/color_detector.h"
#include "detector/detector.h"
#include "detector/number_detector.h"
#include "detector/shading_detector.h"
#include "detector/shape_detector.h"
#include "utils/logger.h"

class SetDebugger
{
public:
    SetDebugger();
    ~SetDebugger();

    void run(uint8_t* image_buffer, int width, int height);

private:
    enum class Status
    {
        OPTIMIZE,
        GENERATE_SET
    };

    Image run(const Image& source_img, const cv::Rect& roi);
    void optimize_filter(const Image& img);
    std::vector<std::pair<Centroid, Image>> generate_card_images(const Image& img);
    Image generate_debug_image(const Image& img, const cv::Rect& roi);

    NumberDetector m_number_detector{};
    ColorDetector m_color_detector{};
    ShadingDetector m_shading_detector{};
    ShapeDetector m_shape_detector{};

    std::optional<Boundaries> m_boundaries{};

    Status m_status{};
    std::atomic_bool m_is_running{};
    size_t m_number_of_cards{};
};
