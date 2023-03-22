
#pragma once

#include <cstdint>

class SetProcessor
{
public:
    // for now, only RGBA color model is supported
    static void run(uint8_t* image_buffer, int width, int height);
    static void debug_run(uint8_t* image_buffer, int width, int height);
    static void save_image(uint8_t* image_buffer, int width, int height, const char* path);
};
