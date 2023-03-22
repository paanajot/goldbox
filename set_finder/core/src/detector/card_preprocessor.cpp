
#include "detector/card_preprocessor.h"

#include <opencv2/imgproc.hpp>

namespace
{
constexpr int DIAMETER = 7;
constexpr double SIGMA_COLOR = 40;
constexpr double SIGMA_SPACE = 40;

constexpr int ERODE_ITERATIONS = 1;
const cv::Point anchor = cv::Point(-1, -1);

constexpr int CONTOUR_AREA_TH = 200;

} // namespace

void CardPreprocessor::run(Image card)
{
    m_input_card = card.clone();
    m_filtered_card = remove_noises(card);
    m_contours = generate_contours(m_filtered_card);
}

Image CardPreprocessor::get_input_card() const
{
    return m_input_card;
}

Image CardPreprocessor::get_filtered_card() const
{
    return m_filtered_card;
}

Contours CardPreprocessor::get_contours() const
{
    return m_contours;
}

Image CardPreprocessor::remove_noises(Image card)
{

    Image result_card{};
    cv::bilateralFilter(card, result_card, DIAMETER, SIGMA_COLOR, SIGMA_SPACE);

    constexpr int MORPH_SIZE = 2;
    const auto element = cv::getStructuringElement(cv::MORPH_RECT,
                                                   cv::Size(2 * MORPH_SIZE + 1, 2 * MORPH_SIZE + 1),
                                                   cv::Point(MORPH_SIZE, MORPH_SIZE));

    cv::erode(result_card, result_card, element, anchor, ERODE_ITERATIONS);

    return result_card;
}

Contours CardPreprocessor::generate_contours(Image card)
{
    constexpr int CANNY_TH1 = 40;
    constexpr int CANNY_TH2 = 120;

    Image contours_card{};
    cv::cvtColor(card, contours_card, cv::COLOR_BGR2GRAY);
    cv::Canny(contours_card, contours_card, CANNY_TH1, CANNY_TH2);

    Contours contours{}, result_contours{};
    cv::findContours(contours_card, contours, {}, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::copy_if(
        contours.cbegin(),
        contours.cend(),
        std::back_inserter(result_contours),
        [](const auto& contour) { return cv::arcLength(contour, true) > CONTOUR_AREA_TH; });

    // TODO: try to use ranges
    // contours | std::views::filter([&](const auto& contour) {
    //     return cv::arcLength(contour, true) > CONTOUR_AREA_TH;
    // })

    return result_contours;
}
