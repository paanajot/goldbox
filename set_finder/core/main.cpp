
#include "core/set_engine.h"

#include "utils/logger.h"

#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

int main()
{
    Logger::enable_color();
    auto stream = cv::VideoCapture(4); // NOLINT

    SetEngine set_engine{};

    if(!stream.isOpened())
    {
        Logger::error("Unable to open camera");
        return -1;
    }

    cv::Mat frame{};
    while(true)
    {
        stream.read(frame);

        if(frame.empty())
        {
            Logger::error("ERROR! blank frame grabbed");
            break;
        }

        frame = set_engine.run(frame);

        cv::imshow("Live", frame);
        if(cv::waitKey(1) >= 0) // NOLINT
            break;
    }

    return 0;
}
