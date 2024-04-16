
#include "core/set_processor.h"

#include "utils/logger.h"

#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

constexpr int FRAME_WIDTH = 1920;
constexpr int FRAME_HEIGHT = 1080;

int main()
{
    Logger::enable_color();
    auto stream = cv::VideoCapture(0); // NOLINT
    stream.set(cv::CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
    stream.set(cv::CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);

    SetProcessor set_processor{};

    if(!stream.isOpened())
    {
        Logger::error("Unable to open camera");
        return -1;
    }

    cv::namedWindow("Live", 0);
    cv::resizeWindow("Live", FRAME_WIDTH / 2, FRAME_HEIGHT / 2);

    cv::Mat frame{};
    while(true)
    {
        stream.read(frame);

        if(frame.empty())
        {
            Logger::error("ERROR! blank frame grabbed");
            break;
        }

        set_processor.run(reinterpret_cast<uint8_t*>(frame.data), FRAME_WIDTH, FRAME_HEIGHT);

        cv::imshow("Live", frame);
        if(cv::waitKey(1) >= 0) // NOLINT
            break;
    }

    return 0;
}
