
#pragma once

#include <atomic>
#include <cstdint>
#include <string>

#include "common/types.h"

class ImageWriter
{
public:
    void save(uint8_t* image_buffer, int width, int height, const std::string& path);

private:
    void run(cv::Mat img, const std::string& path);

    void save_image(const cv::Mat& img, const std::string& path, uint32_t file_count);
    void save_cards(const cv::Mat& img, const std::string& path, uint32_t file_count);

    std::atomic_bool m_is_running{};
};
