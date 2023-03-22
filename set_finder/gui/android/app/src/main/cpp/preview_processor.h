
#pragma once

#include <camera/NdkCameraDevice.h>
#include <camera/NdkCameraError.h>
#include <camera/NdkCameraManager.h>
#include <camera/NdkCameraMetadataTags.h>
#include <media/NdkImageReader.h>

#include "camera_manager.h"

#include <atomic>
#include <string>

class PreviewProcessor
{
public:
    struct PreviewContext
    {
        ANativeWindow* native_window;
        std::atomic_bool& save_image;
        std::atomic_bool& debug_run;
        std::string save_path;
    };

    PreviewProcessor(const ImageSize& image_size,
                     AIMAGE_FORMATS image_format,
                     ANativeWindow* native_window,
                     std::atomic_bool& save_image,
                     std::atomic_bool& debug_run,
                     const char* save_path);
    ~PreviewProcessor();

    ANativeWindow* getSurface();

private:
    AImageReader* createReader(const ImageSize& image_size,
                               AIMAGE_FORMATS image_format,
                               PreviewContext* preview_context);
    ANativeWindow* createSurface(AImageReader* reader);

    PreviewContext m_preview_context;
    AImageReader* m_image_reader;
    ANativeWindow* m_surface;
};
