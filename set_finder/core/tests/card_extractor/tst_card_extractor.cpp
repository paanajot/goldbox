#include <gtest/gtest.h>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <filesystem>
#include <memory>
#include <string>
#include <utility>

#include "card_extractor/filter_optimizer.h"
#include "utils/algorithm.h"
#include "utils/logger.h"

namespace
{
const std::string base_path = "/repo/data/images/card_detector/";
const uint16_t expected_3_cards = 3;
const uint16_t expected_12_cards = 12;
const uint16_t expected_15_cards = 15;
} // namespace

class CardExtractorTest : public testing::TestWithParam<uint16_t>
{ };

TEST_P(CardExtractorTest, validate_number_of_cards)
{
    Logger::enable_color();
    constexpr int WIDTH = 800;
    const auto expected_size = GetParam();
    const auto dir_path = base_path + std::to_string(expected_size);

    const auto image_paths = std::filesystem::directory_iterator(dir_path);

    for(const auto& image_path : image_paths)
    {
        const auto& source_img = cv::imread(image_path.path());
        const auto& resized_img = get_resized_image_keep_ratio(source_img, WIDTH);

        FilterOptimizer filter_optimizer{};
        bool is_optimized = false;

        for(uint16_t i = 0; i < 3; ++i)
        {
            const auto& boundaries = filter_optimizer.optimize(resized_img);
            if(boundaries.has_value() && (expected_size == filter_optimizer.get_contours_size()))
            {
                is_optimized = true;
                break;
            }
        }
        ASSERT_TRUE(is_optimized) << image_path;
    }
}

INSTANTIATE_TEST_SUITE_P(SizeOfExtractedCardsTest,
                         CardExtractorTest,
                         testing::Values(expected_3_cards, expected_12_cards, expected_15_cards));
