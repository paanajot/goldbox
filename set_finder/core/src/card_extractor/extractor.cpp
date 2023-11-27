
#include "extractor.h"

#include "contours_extractor.h"
#include "corners_helper.h"
#include "filter_optimizer.h"
#include "image_filter.h"

#include "utils/algorithm.h"
#include "utils/logger.h"

#include <algorithm>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

namespace
{
constexpr int CARD_WIDTH = 200;
constexpr int CARD_HEIGHT = 133;
constexpr int MARGINS = 5;
constexpr int CROPPED_CARD_WIDTH = CARD_WIDTH - 2 * MARGINS;
constexpr int CROPPED_CARD_HEIGHT = CARD_HEIGHT - 2 * MARGINS;
} // namespace

std::vector<std::pair<Centroid, Image>> Extractor::extract_cards(const Image& source_img,
                                                                 const Boundaries& boundaries) const
{
    const auto& filtered_contours = extract_contours(source_img, boundaries);
    const auto& cards_corners =
        CornersHelper::generate_corners(source_img.size(), filtered_contours);

    if(cards_corners.empty())
    {
        return {};
    }

    std::vector<std::pair<Centroid, Image>> cards_img{};
    for(uint32_t i = 0; i < cards_corners.size(); ++i)
    {
        const auto& card_corners = cards_corners.at(i);
        if(card_corners.size() == 4)
        {
            const auto centroid = get_centroid(filtered_contours.at(i));

            const auto warped_card = get_warped_card(source_img, card_corners);
            const auto cropped_card = Image(
                warped_card, cv::Rect(MARGINS, MARGINS, CROPPED_CARD_WIDTH, CROPPED_CARD_HEIGHT));

            cards_img.emplace_back(std::pair<Centroid, Image>{centroid, cropped_card});
        }
    }

    return cards_img;
}

Contours Extractor::extract_contours(const Image& source_img, const Boundaries& boundaries) const
{
    const auto& [lower, upper] = boundaries;
    const auto filtered_img = ImageFilter{source_img}.filter(lower, upper);

    const ContoursExtractor contours_extractor{filtered_img};
    const auto contours = contours_extractor.find_contours();
    return contours_extractor.filter_contours(contours);
}

Image Extractor::get_warped_card(const Image& source_img, const Corners& corners) const
{
    const auto ordered_corners = CornersHelper::order_corners(corners);
    const auto first_side = cv::norm(ordered_corners[0] - ordered_corners[1]);
    const auto second_side = cv::norm(ordered_corners[0] - ordered_corners[3]);

    Corners2F destination_points{};
    if(first_side > second_side)
    {
        destination_points.push_back({0, 0});
        destination_points.push_back({CARD_WIDTH, 0});
        destination_points.push_back({CARD_WIDTH, CARD_HEIGHT});
        destination_points.push_back({0, CARD_HEIGHT});
    }
    else
    {
        destination_points.push_back({CARD_WIDTH, 0});
        destination_points.push_back({CARD_WIDTH, CARD_HEIGHT});
        destination_points.push_back({0, CARD_HEIGHT});
        destination_points.push_back({0, 0});
    }

    const auto matrix = cv::getPerspectiveTransform(ordered_corners, destination_points);
    Image warped_card{};
    cv::warpPerspective(source_img, warped_card, matrix, cv::Size(CARD_WIDTH, CARD_HEIGHT));
    return warped_card;
}
