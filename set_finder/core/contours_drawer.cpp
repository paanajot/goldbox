
#include "card_extractor/extractor.h"
#include "card_extractor/filter_optimizer.h"
#include "utils/algorithm.h"
#include "utils/logger.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

namespace
{
constexpr int CONTOUR_IDX = -1;
constexpr int THICKNESS = 2;
const cv::Scalar color(0, 255, 0);
} // namespace

int main()
{
    Logger::enable_color();

    [[maybe_unused]] const auto dynamic_filter_card = [&](const auto& img, const auto& iterations) {
        static FilterOptimizer filter_optimizer{};
        for(uint16_t i = 0; i < iterations; ++i)
        {
            const auto boundaries = filter_optimizer.optimize(img);

            if(boundaries)
            {
                const ImageFilter image_filter{img};
                const auto& [lower, upper] = *boundaries;

                Logger::debug(lower, upper);
                const auto& filtered_img = image_filter.filter(lower, upper);

                const auto& filtered_contours = Extractor{}.extract_contours(img, *boundaries);
                cv::drawContours(filtered_img, filtered_contours, CONTOUR_IDX, color, THICKNESS);

                show_image(filtered_img);
            }
        }
    };

    [[maybe_unused]] const auto static_filter_card = [&](const auto& img) {
        const auto v_min = 98;
        const auto s_max = 20;

        const Boundary lower = Boundary{0, 0, v_min};
        const Boundary upper = Boundary{179, s_max, 255};

        const ImageFilter image_filter{img};
        const auto& filtered_img = image_filter.filter(lower, upper);

        const auto& filtered_contours = Extractor{}.extract_contours(img, Boundaries{lower, upper});
        cv::drawContours(filtered_img, filtered_contours, CONTOUR_IDX, color, THICKNESS);
        show_image(filtered_img);
    };

    const std::string path = "/repo/data/images/card_detector/12/20221109_174438.jpg";
    // const std::string path = "/repo/data/images/card_detector/12/20221018_155819.jpg";
    // const std::string path = "/repo/data/images/card_detector/15/20221029_202019.jpg";

    constexpr int IMAGE_WIDTH = 800;
    // auto img = cv::imread(path);
    auto img = get_resized_image_keep_ratio(cv::imread(path), IMAGE_WIDTH);

    dynamic_filter_card(img, 3);
    // static_filter_card(img);

    return 0;
}
