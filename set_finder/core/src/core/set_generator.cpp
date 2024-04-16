
#include "core/set_generator.h"

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

#include <opencv2/imgproc.hpp>

namespace
{
constexpr int CENTROID_DISTANCE_TH = 50;
constexpr int CONTOUR_IDX = -1;
constexpr int THICKNESS = 5;
const cv::Scalar color(0, 255, 0);
} // namespace

SetGenerator::SetGenerator()
{
    m_detectors.emplace_back(std::make_unique<ShapeDetector>());
    m_detectors.emplace_back(std::make_unique<ShadingDetector>());
    m_detectors.emplace_back(std::make_unique<NumberDetector>());
    m_detectors.emplace_back(std::make_unique<ColorDetector>());
}

std::vector<Centroid>
SetGenerator::generate_set(const std::vector<std::pair<Centroid, Image>>& card_imgs) const
{
    CardPreprocessor preprocessor{};
    std::vector<Card> cards{};
    for(const auto& [centroid, card_img] : card_imgs)
    {
        preprocessor.run(card_img);
        const auto features = detect_features(preprocessor);
        if(features.size() == 4)
        {
            cards.emplace_back(Card{centroid, features});
        }
    }

    const auto set_indices = SetFinder::find(cards);
    if(set_indices.has_value())
    {
        std::vector<Centroid> set_points{};
        for(const auto& idx : *set_indices)
        {
            set_points.emplace_back(cards[idx].centroid);
        }
        return set_points;
    }

    return {};
}

std::vector<Feature> SetGenerator::detect_features(const CardPreprocessor& preprocessor) const
{
    std::vector<Feature> features{};
    for(const auto& detector : m_detectors)
    {
        const auto feature = detector->detect_feature(preprocessor);
        if(feature == 0)
        {
            return {};
        }
        features.emplace_back(feature);
    }

    return features;
}

std::vector<Centroid> SetGenerator::update_set(const Image& image,
                                               const Contours& contours,
                                               const std::vector<Centroid>& set_points)
{
    std::vector<Centroid> centroids{};
    for(const auto& contour : contours)
    {
        centroids.emplace_back(get_centroid(contour));
    }

    Contours set_contours{};
    std::vector<Centroid> new_set_points{};
    for(const auto& set_centroid : set_points)
    {
        const auto& point_diff_comparison = [&set_centroid](const auto& lhs, const auto& rhs) {
            return cv::norm(lhs - set_centroid) < cv::norm(rhs - set_centroid);
        };
        const auto& min_distance_it =
            std::min_element(centroids.cbegin(), centroids.cend(), point_diff_comparison);

        if((min_distance_it == centroids.cend()) ||
           (cv::norm(*min_distance_it - set_centroid) > CENTROID_DISTANCE_TH))
        {
            return {};
        }

        const auto closest_point_idx = min_distance_it - centroids.cbegin();
        new_set_points.emplace_back(*min_distance_it);
        set_contours.emplace_back(contours[closest_point_idx]);
    }

    cv::drawContours(image, set_contours, CONTOUR_IDX, color, THICKNESS);

    return new_set_points;
}
