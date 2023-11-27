
#include "debugger/set_debugger.h"

#include "card_extractor/extractor.h"
#include "card_extractor/filter_optimizer.h"
#include "common/card.h"
#include "core/set_finder.h"
#include "detector/card_preprocessor.h"
#include "detector/color_detector.h"
#include "detector/detector.h"
#include "detector/number_detector.h"
#include "detector/shading_detector.h"
#include "detector/shape_detector.h"
#include "utils/algorithm.h"
#include "utils/logger.h"

#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>

#include <functional>
#include <map>
#include <thread>

namespace
{
constexpr int THICKNESS = 3;
constexpr int CONTOUR_IDX = -1;
constexpr int FILL_OPTION = -1;
constexpr int SYMBOL_SPACE = 8;
constexpr int SYMBOL_MARGIN_SPACE = 4;
constexpr int CARD_SPACE = 10;
constexpr int X_SHIFT_OFFSET = 50;

// NOLINTBEGIN
std::map<uint16_t, cv::Scalar> feature_to_color = {
    {1, cv::Scalar(0, 128, 0)}, {2, cv::Scalar(0, 0, 255)}, {3, cv::Scalar(128, 0, 128)}};

std::map<uint16_t, cv::Scalar> feature_to_striped_color = {
    {1, cv::Scalar(144, 238, 144)}, {2, cv::Scalar(203, 204, 255)}, {3, cv::Scalar(255, 185, 227)}};

const auto oval_size = cv::Size(45, 98);
const std::vector<cv::Point> oval_contour = {
    cv::Point{15, 0},  cv::Point{14, 1},  cv::Point{13, 1},  cv::Point{12, 2},  cv::Point{11, 2},
    cv::Point{10, 3},  cv::Point{9, 3},   cv::Point{5, 7},   cv::Point{5, 8},   cv::Point{3, 10},
    cv::Point{3, 11},  cv::Point{2, 12},  cv::Point{2, 14},  cv::Point{1, 15},  cv::Point{1, 19},
    cv::Point{0, 20},  cv::Point{0, 80},  cv::Point{1, 81},  cv::Point{1, 83},  cv::Point{2, 84},
    cv::Point{2, 86},  cv::Point{3, 87},  cv::Point{3, 88},  cv::Point{10, 95}, cv::Point{11, 95},
    cv::Point{12, 96}, cv::Point{14, 96}, cv::Point{15, 97}, cv::Point{27, 97}, cv::Point{28, 96},
    cv::Point{31, 96}, cv::Point{32, 95}, cv::Point{33, 95}, cv::Point{40, 88}, cv::Point{40, 87},
    cv::Point{41, 86}, cv::Point{41, 85}, cv::Point{42, 84}, cv::Point{42, 82}, cv::Point{43, 81},
    cv::Point{43, 76}, cv::Point{44, 75}, cv::Point{44, 18}, cv::Point{43, 17}, cv::Point{43, 14},
    cv::Point{42, 13}, cv::Point{42, 11}, cv::Point{41, 10}, cv::Point{41, 9},  cv::Point{39, 7},
    cv::Point{39, 6},  cv::Point{36, 3},  cv::Point{35, 3},  cv::Point{34, 2},  cv::Point{33, 2},
    cv::Point{32, 1},  cv::Point{31, 1},  cv::Point{30, 0}};

const auto squiggle_size = cv::Size(44, 93);
const std::vector<cv::Point> squiggle_contour = {
    cv::Point{10, 0},  cv::Point{9, 1},   cv::Point{6, 1},   cv::Point{4, 3},   cv::Point{3, 3},
    cv::Point{2, 4},   cv::Point{2, 5},   cv::Point{1, 6},   cv::Point{1, 8},   cv::Point{0, 9},
    cv::Point{0, 15},  cv::Point{1, 16},  cv::Point{1, 18},  cv::Point{2, 19},  cv::Point{2, 20},
    cv::Point{4, 22},  cv::Point{4, 23},  cv::Point{5, 24},  cv::Point{5, 25},  cv::Point{6, 26},
    cv::Point{6, 27},  cv::Point{7, 28},  cv::Point{7, 30},  cv::Point{8, 31},  cv::Point{8, 37},
    cv::Point{7, 38},  cv::Point{7, 40},  cv::Point{6, 41},  cv::Point{6, 43},  cv::Point{5, 44},
    cv::Point{5, 45},  cv::Point{4, 46},  cv::Point{4, 49},  cv::Point{3, 50},  cv::Point{3, 71},
    cv::Point{4, 72},  cv::Point{4, 75},  cv::Point{5, 76},  cv::Point{5, 77},  cv::Point{6, 78},
    cv::Point{6, 79},  cv::Point{7, 80},  cv::Point{7, 81},  cv::Point{14, 88}, cv::Point{15, 88},
    cv::Point{16, 89}, cv::Point{17, 89}, cv::Point{18, 90}, cv::Point{19, 90}, cv::Point{20, 91},
    cv::Point{23, 91}, cv::Point{24, 92}, cv::Point{35, 92}, cv::Point{36, 91}, cv::Point{38, 91},
    cv::Point{39, 90}, cv::Point{40, 90}, cv::Point{43, 87}, cv::Point{43, 77}, cv::Point{42, 76},
    cv::Point{42, 75}, cv::Point{41, 74}, cv::Point{41, 73}, cv::Point{39, 71}, cv::Point{39, 70},
    cv::Point{38, 69}, cv::Point{38, 68}, cv::Point{37, 67}, cv::Point{37, 66}, cv::Point{36, 65},
    cv::Point{36, 62}, cv::Point{35, 61}, cv::Point{35, 55}, cv::Point{36, 54}, cv::Point{36, 52},
    cv::Point{37, 51}, cv::Point{37, 49}, cv::Point{38, 48}, cv::Point{38, 46}, cv::Point{39, 45},
    cv::Point{39, 44}, cv::Point{40, 43}, cv::Point{40, 40}, cv::Point{41, 39}, cv::Point{41, 36},
    cv::Point{42, 35}, cv::Point{42, 16}, cv::Point{41, 15}, cv::Point{41, 14}, cv::Point{40, 13},
    cv::Point{40, 11}, cv::Point{38, 9},  cv::Point{38, 8},  cv::Point{35, 5},  cv::Point{34, 5},
    cv::Point{32, 3},  cv::Point{30, 3},  cv::Point{29, 2},  cv::Point{28, 2},  cv::Point{27, 1},
    cv::Point{25, 1},  cv::Point{24, 0}};

const auto diamond_size = cv::Size(48, 104);
const std::vector<cv::Point> diamond_contour = {
    cv::Point{21, 0},   cv::Point{21, 1},   cv::Point{20, 3},   cv::Point{19, 4},
    cv::Point{19, 5},   cv::Point{18, 8},   cv::Point{17, 9},   cv::Point{17, 10},
    cv::Point{16, 12},  cv::Point{15, 13},  cv::Point{15, 14},  cv::Point{14, 17},
    cv::Point{13, 18},  cv::Point{13, 19},  cv::Point{12, 21},  cv::Point{11, 22},
    cv::Point{11, 24},  cv::Point{10, 26},  cv::Point{9, 27},   cv::Point{9, 28},
    cv::Point{8, 30},   cv::Point{7, 31},   cv::Point{7, 33},   cv::Point{6, 35},
    cv::Point{5, 36},   cv::Point{5, 37},   cv::Point{4, 39},   cv::Point{3, 40},
    cv::Point{3, 42},   cv::Point{2, 44},   cv::Point{1, 45},   cv::Point{1, 47},
    cv::Point{0, 56},   cv::Point{1, 57},   cv::Point{1, 58},   cv::Point{2, 60},
    cv::Point{3, 61},   cv::Point{3, 62},   cv::Point{4, 65},   cv::Point{5, 66},
    cv::Point{5, 67},   cv::Point{6, 70},   cv::Point{7, 71},   cv::Point{7, 72},
    cv::Point{8, 74},   cv::Point{9, 75},   cv::Point{9, 76},   cv::Point{10, 78},
    cv::Point{11, 79},  cv::Point{11, 81},  cv::Point{12, 83},  cv::Point{13, 84},
    cv::Point{13, 86},  cv::Point{14, 88},  cv::Point{15, 89},  cv::Point{15, 90},
    cv::Point{16, 92},  cv::Point{17, 93},  cv::Point{17, 94},  cv::Point{18, 97},
    cv::Point{19, 98},  cv::Point{19, 100}, cv::Point{20, 102}, cv::Point{21, 103},
    cv::Point{26, 103}, cv::Point{27, 101}, cv::Point{27, 100}, cv::Point{28, 99},
    cv::Point{29, 97},  cv::Point{29, 96},  cv::Point{30, 95},  cv::Point{31, 92},
    cv::Point{31, 90},  cv::Point{33, 88},  cv::Point{34, 85},  cv::Point{34, 84},
    cv::Point{35, 83},  cv::Point{36, 81},  cv::Point{36, 79},  cv::Point{37, 78},
    cv::Point{38, 76},  cv::Point{38, 74},  cv::Point{39, 73},  cv::Point{40, 71},
    cv::Point{40, 70},  cv::Point{41, 69},  cv::Point{43, 65},  cv::Point{43, 63},
    cv::Point{44, 62},  cv::Point{45, 59},  cv::Point{45, 58},  cv::Point{46, 57},
    cv::Point{47, 55},  cv::Point{47, 49},  cv::Point{46, 48},  cv::Point{45, 45},
    cv::Point{45, 44},  cv::Point{44, 43},  cv::Point{43, 40},  cv::Point{43, 39},
    cv::Point{42, 38},  cv::Point{40, 34},  cv::Point{40, 32},  cv::Point{39, 31},
    cv::Point{38, 29},  cv::Point{38, 28},  cv::Point{37, 27},  cv::Point{36, 24},
    cv::Point{36, 22},  cv::Point{35, 21},  cv::Point{33, 18},  cv::Point{33, 16},
    cv::Point{32, 15},  cv::Point{31, 13},  cv::Point{31, 11},  cv::Point{30, 10},
    cv::Point{29, 8},   cv::Point{29, 6},   cv::Point{28, 5},   cv::Point{27, 3},
    cv::Point{27, 2},   cv::Point{26, 1}};
// NOLINTEND

const std::map<uint16_t, std::pair<std::vector<cv::Point>, cv::Size>> feature_to_shape_contour = {
    {ShapeDetector::DIAMOND, {diamond_contour, diamond_size}},
    {ShapeDetector::OVAL, {oval_contour, oval_size}},
    {ShapeDetector::SQUIGGLE, {squiggle_contour, squiggle_size}}};
} // namespace

SetDebugger::SetDebugger()
    : m_status(Status::OPTIMIZE)
{ }

SetDebugger::~SetDebugger() = default;

void SetDebugger::run(uint8_t* image_buffer, int width, int height)
{
    const auto blind_spot_width = (width - height) / 2;
    cv::Mat frame = cv::Mat(height, width, CV_8UC4, image_buffer);

    const cv::Scalar color(0, 255, 0);

    cv::line(frame,
             cv::Point(blind_spot_width, 0),
             cv::Point(blind_spot_width, height),
             color,
             THICKNESS);
    cv::line(frame,
             cv::Point(width - blind_spot_width, 0),
             cv::Point(width - blind_spot_width, height),
             color,
             THICKNESS);

    if(m_is_running)
    {
        return;
    }

    const auto roi = cv::Rect(blind_spot_width, 0, width - 2 * blind_spot_width, height);

    cv::Mat bgr_frame{};
    cv::cvtColor(frame, bgr_frame, cv::COLOR_RGBA2BGR);

    auto result_img = run(bgr_frame, roi);

    cv::cvtColor(result_img, frame, cv::COLOR_BGR2RGBA);
}

Image SetDebugger::run(const Image& source_img, const cv::Rect& roi)
{
    if(!m_is_running)
    {
        if(m_status == Status::OPTIMIZE)
        {
            Logger::info("optimize filter");
            auto roi_frame = cv::Mat(source_img, roi);
            std::thread(&SetDebugger::optimize_filter, this, roi_frame).detach();
        }
        else if(m_status == Status::GENERATE_SET)
        {
            Logger::info("generate set");
            return generate_debug_image(source_img, roi);
        }
    }
    return source_img;
}

void SetDebugger::optimize_filter(const Image& img)
{
    m_is_running = true;
    static FilterOptimizer filter_optimizer{};
    m_boundaries = filter_optimizer.optimize(img);
    if(m_boundaries)
    {
        m_number_of_cards = filter_optimizer.get_contours_size();
        m_status = Status::GENERATE_SET;
    }
    m_is_running = false;
}

std::vector<std::pair<Centroid, Image>> SetDebugger::generate_card_images(const Image& img)
{
    const Extractor extractor{};
    const auto& card_imgs = extractor.extract_cards(img, m_boundaries.value());
    if(m_number_of_cards != card_imgs.size())
    {
        return {};
    }

    return card_imgs;
}

// NOLINTBEGIN [bugprone-easily-swappable-parameters]
void draw_detected_card(Image& card_img,
                        uint16_t number_of_shapes,
                        uint16_t color_feature,
                        uint16_t shading_feature,
                        uint16_t shape_feature)
// NOLINTEND
{
    const auto color = feature_to_color.at(color_feature);
    const auto striped_color = feature_to_striped_color.at(color_feature);
    const auto contour_width = card_img.size().width / 15;

    const auto fill_color = (shading_feature == ShadingDetector::STRIPED) ? striped_color : color;
    const bool is_fill_needed =
        shading_feature == ShadingDetector::STRIPED or shading_feature == ShadingDetector::SOLID;

    const auto draw_symbol =
        [&](const auto& symbol_contour, const auto& old_width, const auto& offset) {
            const auto resized_contour =
                transform_contour(symbol_contour, offset, old_width, contour_width);

            if(is_fill_needed)
            {
                cv::drawContours(
                    card_img, Contours{resized_contour}, CONTOUR_IDX, fill_color, FILL_OPTION);
            }
            cv::drawContours(card_img, Contours{resized_contour}, CONTOUR_IDX, color, THICKNESS);
        };

    const auto& [symbol_contour, symbol_size] = feature_to_shape_contour.at(shape_feature);

    for(uint16_t i = 0; i < number_of_shapes; ++i)
    {
        cv::Point offset{SYMBOL_MARGIN_SPACE + (contour_width + SYMBOL_SPACE) * i,
                         SYMBOL_MARGIN_SPACE};
        draw_symbol(symbol_contour, symbol_size.width, offset);
    }
}

Image SetDebugger::generate_debug_image(const Image& img, const cv::Rect& roi)
{
    auto roi_frame = cv::Mat(img, roi);

    const auto card_imgs = generate_card_images(roi_frame);
    Image debug_img = cv::Mat::zeros(img.size(), CV_8UC3);
    if(card_imgs.empty())
    {
        m_status = Status::OPTIMIZE;
        return debug_img;
    }

    const int new_card_width = (img.size().height - 30) / 2;

    CardPreprocessor preprocessor{};
    for(size_t i = 0; i < card_imgs.size(); ++i)
    {
        auto card_img = card_imgs.at(i).second;
        preprocessor.run(card_img);
        const auto number_of_shapes = m_number_detector.detect_feature(preprocessor);
        const auto color_feature = m_color_detector.detect_feature(preprocessor);
        const auto shading_feature = m_shading_detector.detect_feature(preprocessor);
        const auto shape_feature = m_shape_detector.detect_feature(preprocessor);

        if(number_of_shapes == 0 or color_feature == 0 or shading_feature == 0 or
           shape_feature == 0)
        {
            continue;
        }

        auto card = get_resized_image_keep_ratio(card_img, new_card_width);
        draw_detected_card(card, number_of_shapes, color_feature, shading_feature, shape_feature);

        cv::rotate(card, card, cv::ROTATE_90_COUNTERCLOCKWISE);
        const auto x_shift = (card.size().width + CARD_SPACE) * (i / 2) + X_SHIFT_OFFSET;
        const auto y_shift = (card.size().height + CARD_SPACE) * (i % 2) + CARD_SPACE;

        if(static_cast<int>(x_shift + card.size().width) >= img.size().width)
        {
            Logger::warning("at least one card out of the screen");
            break;
        }
        cv::Rect roi(cv::Point(static_cast<int>(x_shift), static_cast<int>(y_shift)), card.size());
        card.copyTo(debug_img(roi));
    }

    return debug_img;
}
