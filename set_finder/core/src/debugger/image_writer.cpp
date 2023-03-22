

#include "debugger/image_writer.h"

#include <opencv2/core/types.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <cstdint>
#include <filesystem>
#include <thread>

#include "card_extractor/extractor.h"
#include "card_extractor/filter_optimizer.h"
#include "utils/logger.h"

void ImageWriter::save(uint8_t* image_buffer, int width, int height, const std::string& path)
{
    cv::Mat frame = cv::Mat(height, width, CV_8UC4, image_buffer);
    // It's important to pass an image copy to a new thread
    std::thread(&ImageWriter::run, this, frame.clone(), path).detach();
}

void ImageWriter::run(cv::Mat img, const std::string& path)
{
    if(!m_is_running)
    {
        m_is_running = true;
        const auto width = img.size().width;
        const auto height = img.size().height;

        if(!std::filesystem::exists(path))
        {
            Logger::warning(path, "doesn't exists");
            m_is_running = true;
        }

        const auto blind_spot_width = (width - height) / 2;
        const auto roi = cv::Rect(blind_spot_width, 0, width - 2 * blind_spot_width, height);
        const auto roi_frame = cv::Mat(img, roi);

        cv::Mat bgr_roi_frame{};
        cv::cvtColor(roi_frame, bgr_roi_frame, cv::COLOR_RGBA2BGR);

        const auto files_counter = [](const auto& iter) -> uint32_t {
            return std::count_if(
                begin(iter), end(iter), [](const auto& file) { return file.is_regular_file(); });
        };

        const std::string images_path = path + "/images/";
        std::filesystem::create_directory(images_path);

        const auto images_iter = std::filesystem::directory_iterator(images_path);
        const auto images_count = files_counter(images_iter);

        const std::string cards_path = path + "/cards/";
        std::filesystem::create_directory(cards_path);

        const auto cards_iter = std::filesystem::directory_iterator(cards_path);
        const auto cards_count = files_counter(cards_iter);

        save_image(bgr_roi_frame, images_path, images_count);
        save_cards(bgr_roi_frame, cards_path, cards_count);
        m_is_running = false;
    }
}

void ImageWriter::save_image(const cv::Mat& img, const std::string& path, uint32_t file_count)
{
    const auto image_path = path + std::to_string(file_count) + ".jpeg";
    const auto save_status = cv::imwrite(image_path, img);
    if(save_status == 1)
    {
        Logger::info("image saved:", image_path);
    }
    else
    {
        Logger::error("couldn't save an image:", image_path);
    }
}

void ImageWriter::save_cards(const cv::Mat& img, const std::string& path, uint32_t file_count)
{
    static FilterOptimizer filter_optimizer{};
    const auto boundaries = filter_optimizer.optimize(img);

    if(boundaries)
    {
        const Extractor extractor{};
        const auto& card_imgs = extractor.extract_cards(img, boundaries.value());
        for(const auto& [_, card_img] : card_imgs)
        {
            const auto card_path = path + std::to_string(file_count++) + ".jpeg";
            const auto save_status = cv::imwrite(card_path, card_img);
            if(save_status == 1)
            {
                Logger::info("card saved:", card_path);
            }
            else
            {
                Logger::error("couldn't save a card:", card_path);
            }
        }
    }
}
