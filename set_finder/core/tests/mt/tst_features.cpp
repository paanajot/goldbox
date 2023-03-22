#include <gtest/gtest.h>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <filesystem>
#include <memory>
#include <string>
#include <utility>

#include "card_extractor/extractor.h"
#include "card_extractor/filter_optimizer.h"
#include "detector/card_preprocessor.h"
#include "detector/number_detector.h"
#include "detector/shading_detector.h"
#include "detector/shape_detector.h"

#include "common/types.h"
#include "utils/algorithm.h"
#include "utils/logger.h"

namespace
{
constexpr size_t VALID_NUMBER_12 = 12;
constexpr size_t VALID_NUMBER_15 = 15;

const std::string base_path = "/repo/data/images/";
const std::string number_path = base_path + "number_detector/test/";
const std::string shape_path = base_path + "shape_detector/test/";
const std::string shading_path = base_path + "shading_detector/test/";
} // namespace

struct TestData
{
    std::string dir_path{};
    uint16_t feature{};
    std::shared_ptr<Detector> detector{};
};

::std::ostream& operator<<(::std::ostream& os, const TestData& test_data)
{
    return os << test_data.dir_path;
}

const auto number_detector = std::make_shared<NumberDetector>();
const auto shape_detector = std::make_shared<ShapeDetector>();
const auto shading_detector = std::make_shared<ShadingDetector>();

const TestData data_1{number_path + "one_inputs", 1, number_detector};
const TestData data_2{number_path + "two_inputs", 2, number_detector};
const TestData data_3{number_path + "three_inputs", 3, number_detector};

const TestData data_4{shape_path + "diamond_inputs", ShapeDetector::DIAMOND, shape_detector};
const TestData data_5{shape_path + "oval_inputs", ShapeDetector::OVAL, shape_detector};
const TestData data_6{shape_path + "squiggle_inputs", ShapeDetector::SQUIGGLE, shape_detector};

// clang-format off
const TestData data_7{shading_path + "solid_inputs", ShadingDetector::SOLID, shading_detector};
const TestData data_8{shading_path + "outlined_inputs", ShadingDetector::OUTLINED, shading_detector};
const TestData data_9{shading_path + "striped_inputs", ShadingDetector::STRIPED, shading_detector};
// clang-format on

class FeaturesTest : public testing::TestWithParam<TestData>
{
public:
    std::vector<std::pair<cv::Point, Image>> get_card_images(const std::string& dir_path)
    {
        constexpr int WIDTH = 800;

        const auto source_img = cv::imread(dir_path);
        const auto resized_img = get_resized_image_keep_ratio(source_img, WIDTH);

        FilterOptimizer filter_optimizer{};
        for(uint16_t i = 0; i < 3; ++i)
        {
            const auto boundaries = filter_optimizer.optimize(resized_img);
            if(!boundaries.has_value())
            {
                Logger::error("dir_path:", dir_path);
                assert(false);
            }

            if(filter_optimizer.get_contours_size() == VALID_NUMBER_12 ||
               filter_optimizer.get_contours_size() == VALID_NUMBER_15)
            {
                return Extractor{}.extract_cards(resized_img, *boundaries);
            }
        }

        return {};
    }
};

TEST_P(FeaturesTest, validate_feature)
{
    Logger::enable_color();
    const auto params = GetParam();
    const auto feature = params.feature;
    const auto detector = params.detector;
    const auto dir_path = params.dir_path;

    const auto image_paths = std::filesystem::directory_iterator(dir_path);

    CardPreprocessor preprocessor{};
    for(const auto& image_path : image_paths)
    {
        if(std::string{image_path.path().filename()}.starts_with("fails"))
        {
            continue;
        }

        const auto card_imgs = FeaturesTest::get_card_images(image_path.path());
        ASSERT_TRUE(!card_imgs.empty());

        for(const auto& [_, card] : card_imgs)
        {
            preprocessor.run(card);
            EXPECT_EQ(detector->detect_feature(preprocessor), feature) << image_path.path();
        }
    }
}

INSTANTIATE_TEST_SUITE_P(
    FeaturesMt,
    FeaturesTest,
    testing::Values(data_1, data_2, data_3, data_4, data_5, data_6, data_7, data_8, data_9));
