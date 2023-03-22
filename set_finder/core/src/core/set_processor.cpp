
#include "core/set_processor.h"
#include "core/set_engine.h"
#include "debugger/image_writer.h"
#include "debugger/set_debugger.h"

#include "utils/color.h"

void SetProcessor::run(uint8_t* image_buffer, int width, int height)
{
    static SetEngine set_engine{};
    return set_engine.run(image_buffer, width, height);
}

void SetProcessor::debug_run(uint8_t* image_buffer, int width, int height)
{
    static SetDebugger set_debugger{};
    return set_debugger.run(image_buffer, width, height);
}

void SetProcessor::save_image(uint8_t* image_buffer, int width, int height, const char* path)
{
    static ImageWriter image_writer{};
    return image_writer.save(image_buffer, width, height, std::string{path});
}
