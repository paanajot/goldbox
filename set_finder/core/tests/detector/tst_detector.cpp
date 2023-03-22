#include <gtest/gtest.h>

#include <opencv2/imgcodecs.hpp>

#include <filesystem>
#include <memory>
#include <string>
#include <utility>

#include "detector/card_preprocessor.h"
#include "detector/color_detector.h"
#include "detector/number_detector.h"
#include "detector/shading_detector.h"
#include "detector/shape_detector.h"
#include "utils/logger.h"

const std::string base_path = "/repo/data/images/";
const std::string number_path = base_path + "number_detector/test/";
const std::string shape_path = base_path + "shape_detector/test/";
const std::string shading_path = base_path + "shading_detector/test/";
const std::string color_path = base_path + "color_detector/test/";
const std::string color_path_app = base_path + "color_detector/app_test/";

struct TestData
{
    std::string dir_path{};
    uint16_t feature_number{};
    std::shared_ptr<Detector> detector{};
};

::std::ostream& operator<<(::std::ostream& os, const TestData& test_data)
{
    return os << test_data.dir_path;
}

const auto number_detector = std::make_shared<NumberDetector>();
const auto shape_detector = std::make_shared<ShapeDetector>();
const auto shading_detector = std::make_shared<ShadingDetector>();
const auto color_detector = std::make_shared<ColorDetector>();

const TestData data_1{number_path + "one", 1, number_detector};
const TestData data_2{number_path + "two", 2, number_detector};
const TestData data_3{number_path + "three", 3, number_detector};

const TestData data_4{shape_path + "diamond", ShapeDetector::DIAMOND, shape_detector};
const TestData data_5{shape_path + "oval", ShapeDetector::OVAL, shape_detector};
const TestData data_6{shape_path + "squiggle", ShapeDetector::SQUIGGLE, shape_detector};

const TestData data_7{shading_path + "solid", ShadingDetector::SOLID, shading_detector};
const TestData data_8{shading_path + "outlined", ShadingDetector::OUTLINED, shading_detector};
const TestData data_9{shading_path + "striped", ShadingDetector::STRIPED, shading_detector};

const TestData data_10{color_path + "green", 1, color_detector};
const TestData data_11{color_path + "red", 2, color_detector};
const TestData data_12{color_path + "purple", 3, color_detector};

const TestData data_13{color_path_app + "green", 1, color_detector};
const TestData data_14{color_path_app + "red", 2, color_detector};
const TestData data_15{color_path_app + "purple", 3, color_detector};

class DetectorTest : public testing::TestWithParam<TestData>
{ };

TEST_P(DetectorTest, validate_feature)
{
    const auto params = GetParam();
    const auto feature_number = params.feature_number;
    const auto detector = params.detector;
    const auto dir_path = params.dir_path;

    const auto image_paths = std::filesystem::directory_iterator(dir_path);
    CardPreprocessor preprocessor{};
    for(const auto& image_path : image_paths)
    {
        const auto card = cv::imread(image_path.path());
        preprocessor.run(card);

        EXPECT_EQ(detector->detect_feature(preprocessor), feature_number) << image_path.path();
    }
}

INSTANTIATE_TEST_SUITE_P(AllDetectorsTest,
                         DetectorTest,
                         testing::Values(data_1,
                                         data_2,
                                         data_3,
                                         data_4,
                                         data_5,
                                         data_6,
                                         data_7,
                                         data_8,
                                         data_9,
                                         data_10,
                                         data_11,
                                         data_12,
                                         data_13,
                                         data_14,
                                         data_15));
