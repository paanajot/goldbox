
#include "core/set_engine.h"

#include "card_extractor/extractor.h"
#include "card_extractor/filter_optimizer.h"
#include "common/card.h"
#include "core/set_generator.h"
#include "utils/algorithm.h"
#include "utils/logger.h"

#include <opencv2/imgproc.hpp>

#include <map>
#include <thread>

namespace
{
const auto get_blind_spot = [](const auto width, const auto height) {
    return (width - height) / 2;
};

const cv::Size2i SIMPLIFIED_FRAME_SIZE{512, 512};

const std::map<EngineTask, cv::Scalar> task_to_color{
    {EngineTask::NONE, cv::Scalar{0, 185, 227}},
    {EngineTask::OPTIMIZE, cv::Scalar{0, 185, 227}},
    {EngineTask::GENERATE_SET, cv::Scalar{0, 227, 169}},
    {EngineTask::UPDATE_CONTOURS, cv::Scalar{0, 255, 0}}};
} // namespace

/* ANDROID
    Image frame = Image(cv::Size(width, height), CV_8UC4, image_buffer);
    // ...
    Image bgr_frame{};
    cv::cvtColor(roi_frame, bgr_frame, cv::COLOR_RGB2BGR);

    auto result_img = run(roi_frame);
    cv::cvtColor(result_img, roi_frame, cv::COLOR_BGR2RGB);
*/
void SetEngine::run(uint8_t* image_buffer, int width, int height)
{
    Image frame = Image(cv::Size(width, height), CV_8UC3, image_buffer);

    draw_limit_lines(frame, width, height);

    if(!m_supervisor.is_none())
    {
        const auto blind_spot_width = get_blind_spot(width, height);
        const auto roi = cv::Rect(blind_spot_width, 0, width - 2 * blind_spot_width, height);
        std::ignore = run(Image(frame, roi));
    }
}

Image SetEngine::run(const Image& img)
{
    switch(m_supervisor.fetch_task())
    {
    case EngineTask::OPTIMIZE: {
        Logger::info("optimize filter");
        std::thread(&SetEngine::optimize_filter, this, img.clone()).detach();
        break;
    }
    case EngineTask::GENERATE_SET: {
        Logger::info("generate set");
        std::thread(&SetEngine::generate_set, this, img.clone()).detach();
        break;
    }
    case EngineTask::UPDATE_CONTOURS: {
        Logger::info("update set");
        return update_set(img);
        break;
    }
    case EngineTask::NONE:
        break;
    }
    return img;
}

void SetEngine::optimize_filter(const Image& img)
{
    auto task = EngineTask::OPTIMIZE;
    Image resized_img;
    cv::resize(img, resized_img, SIMPLIFIED_FRAME_SIZE);

    static FilterOptimizer filter_optimizer{};
    m_boundaries = filter_optimizer.optimize(resized_img);
    if(m_boundaries)
    {
        m_number_of_cards = filter_optimizer.get_contours_size();
        task = EngineTask::GENERATE_SET;
    }

    m_supervisor.set_task(task);
}

void SetEngine::generate_set(const Image& img)
{
    auto task = EngineTask::OPTIMIZE;

    static const Extractor extractor{};
    const auto& card_imgs = extractor.extract_cards(img, m_boundaries.value());
    if(m_number_of_cards == card_imgs.size())
    {
        const static SetGenerator set_generator{};
        m_set = set_generator.generate_set(card_imgs);
        if(!m_set.empty())
        {
            task = EngineTask::UPDATE_CONTOURS;
        }
    }

    m_supervisor.set_task(task);
}

Image SetEngine::update_set(const Image& img)
{
    auto task = EngineTask::UPDATE_CONTOURS;

    static const Extractor extractor{};
    const auto& contours = extractor.extract_contours(img, m_boundaries.value());
    m_set = SetGenerator::update_set(img, contours, m_set);
    if(m_set.empty())
    {
        task = EngineTask::OPTIMIZE;
    }

    m_supervisor.set_task(task);
    return img;
}

void SetEngine::draw_limit_lines(const Image& frame, int width, int height)
{
    constexpr int THICKNESS = 3;
    const auto color = task_to_color.at(m_supervisor.get_current_task());
    const auto blind_spot_width = get_blind_spot(width, height);

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
}
