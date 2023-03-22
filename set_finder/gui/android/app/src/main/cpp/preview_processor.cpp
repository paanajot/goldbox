
#include "preview_processor.h"

#include <android/native_window.h>
#include <camera/NdkCameraDevice.h>
#include <camera/NdkCameraError.h>
#include <camera/NdkCameraManager.h>
#include <camera/NdkCameraMetadataTags.h>
#include <media/NdkImage.h>
#include <media/NdkImageReader.h>

#include "utils/camera_utils.h"
#include "utils/native_debug.h"

#include "core/logger.h"
#include "core/set_processor.h"

#include "libyuv.h"

#include <memory>

class AndroidPrinter : public IPrinter
{
public:
    virtual void print(const char* msg) const override
    {
        LOGI("%s", msg);
    }
};

PreviewProcessor::PreviewProcessor(const ImageSize& image_size,
                                   AIMAGE_FORMATS image_format,
                                   ANativeWindow* native_window,
                                   std::atomic_bool& save_image,
                                   std::atomic_bool& debug_run,
                                   const char* save_path)
    : m_preview_context({native_window, save_image, debug_run, save_path})
    , m_image_reader(PreviewProcessor::createReader(image_size, image_format, &m_preview_context))
    , m_surface(createSurface(m_image_reader))
{
    Logger::init(std::make_unique<AndroidPrinter>());
}

static void imageCallback(void* context, AImageReader* reader)
{
    AImage* image = nullptr;
    const auto status = AImageReader_acquireLatestImage(reader, &image);

    if(status != AMEDIA_OK)
    {
        return;
    }

    int32_t src_width{};
    int32_t src_height{};
    int32_t format{};

    AImage_getWidth(image, &src_width);
    AImage_getHeight(image, &src_height);
    AImage_getFormat(image, &format);

    int32_t number_of_planes{};
    AImage_getNumberOfPlanes(image, &number_of_planes);

    auto preview_context = static_cast<PreviewProcessor::PreviewContext*>(context);
    auto output_surface = preview_context->native_window;
    ANativeWindow_acquire(output_surface);
    ANativeWindow_Buffer buffer;
    ANativeWindow_setBuffersGeometry(
        output_surface, src_width, src_height, AHARDWAREBUFFER_FORMAT_R8G8B8A8_UNORM);

    if(int32_t err = ANativeWindow_lock(output_surface, &buffer, NULL))
    {
        LOGE("ANativeWindow_lock failed with error code: %d\n", err);
        ANativeWindow_release(output_surface);
        return;
    }

    int32_t pixel_stride{};
    int32_t row_stride_0{};
    int32_t row_stride_1{};
    int32_t row_stride_2{};

    AImage_getPlanePixelStride(image, 1, &pixel_stride);
    AImage_getPlaneRowStride(image, 0, &row_stride_0);
    AImage_getPlaneRowStride(image, 1, &row_stride_1);
    AImage_getPlaneRowStride(image, 2, &row_stride_2);

    int data_length_0{};
    uint8_t* image_bits_0{};
    AImage_getPlaneData(image, 0, &image_bits_0, &data_length_0);

    int data_length_1{};
    uint8_t* image_bits_1{};
    AImage_getPlaneData(image, 1, &image_bits_1, &data_length_1);

    int data_length_2{};
    uint8_t* image_bits_2{};
    AImage_getPlaneData(image, 2, &image_bits_2, &data_length_2);

    uint8_t* native_window_bits = reinterpret_cast<uint8_t*>(buffer.bits);
    libyuv::Android420ToABGR(image_bits_0,
                             row_stride_0,
                             image_bits_1,
                             row_stride_1,
                             image_bits_2,
                             row_stride_2,
                             pixel_stride,
                             native_window_bits,
                             buffer.stride * 4,
                             src_width,
                             src_height);

    if(preview_context->debug_run) {
        SetProcessor::debug_run(native_window_bits, src_width, src_height);
        if(preview_context->save_image)
        {
            preview_context->save_image = false;
            SetProcessor::save_image(
                    native_window_bits, src_width, src_height, preview_context->save_path.c_str());
        }
    } else {
        SetProcessor::run(native_window_bits, src_width, src_height);
    }

    ANativeWindow_setBuffersTransform(output_surface, ANATIVEWINDOW_TRANSFORM_ROTATE_90);
    ANativeWindow_unlockAndPost(output_surface);
    AImage_delete(image);
}

AImageReader* PreviewProcessor::createReader(const ImageSize& image_size,
                                             AIMAGE_FORMATS image_format,
                                             PreviewContext* preview_context)
{
    AImageReader* reader = nullptr;
    AImageReader_new(image_size.width, image_size.height, image_format, 4, &reader);

    //if (status != AMEDIA_OK)
    // Handle errors here

    AImageReader_ImageListener listener{
        .context = preview_context,
        .onImageAvailable = imageCallback,
    };

    AImageReader_setImageListener(reader, &listener);

    return reader;
}

ANativeWindow* PreviewProcessor::createSurface(AImageReader* reader)
{
    ANativeWindow* nativeWindow;
    AImageReader_getWindow(reader, &nativeWindow);

    return nativeWindow;
}

ANativeWindow* PreviewProcessor::getSurface()
{
    return m_surface;
}

PreviewProcessor::~PreviewProcessor()
{
    if(m_image_reader)
    {
        AImageReader_delete(m_image_reader);
    }
}
