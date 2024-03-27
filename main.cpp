#include "src/common/common.hpp"
#include "src/http/advice/httpAdvice.hpp"
#include "src/serial/SerialBase.hpp"
#include "src/v5lite/MultiVideoCapture.hpp"
#include "src/v5lite/V5Lite.hpp"
#include <boost/version.hpp>
#include <drogon/drogon.h>
#include <opencv2/core/utils/logger.hpp>

#include <fstream>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <sstream>

using namespace cpptide::serial;
using namespace cpptide::YOLO;
using namespace cpptide::http::advice;
using namespace drogon;


void testMultiVideoCapture()
{
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_WARNING);

    cpptide::YOLO::MultiVideoCapture mvc;
    // mvc.init();
    cv::VideoCapture &cap = mvc.get(1);
    if (!cap.isOpened()) {
        std::cerr << "Error: Cannot open camera" << std::endl;
        cap.open(1);
    }
    cv::Mat frame;
    while (true) {
        cap >> frame;
        if (frame.empty()) {
            std::cerr << "Error: Cannot grab frame" << std::endl;
            break;
        }
        cv::imshow("Live", frame);
        if (cv::waitKey(5) >= 0)// Break on ESC
            break;
    }
    cv::destroyAllWindows();
}

void testOpencvDnn()
{
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_WARNING);

    cv::VideoCapture cap(0);
    // cap.set(cv::CAP_PROP_FRAME_WIDTH, 320);
    // cap.set(cv::CAP_PROP_FRAME_HEIGHT, 240);

    cpptide::YOLO::Net_config config { (float) 0.8, (float) 0.4, (float) 0.5, "E:\\Code\\CppTide\\weights\\v5lite-s.onnx", "E:\\Code\\CppTide\\weights\\class\\coco.names" };
    cpptide::YOLO::V5Lite yolov5(config);

    while (cap.isOpened()) {
        cv::Mat frame;
        cap >> frame;// Capture a frame
        yolov5.detect(frame);
        cv::imshow("Live", frame);
        if (cv::waitKey(1) == 27) {// ESC key
            break;
        }
    }
    cv::destroyAllWindows();
}

void testParseGps()
{
    std::string data = "$GNGGA,121252.000,3937.3032,N,11611.6046,E,1,05,2.0,45.9,M,-5.7,M,,0000*77";
    auto root        = cpptide::common::parseGPSToJson(data);
    std::cout << root.toStyledString() << std::endl;
}
int main()
{

    // testOpencv();
    // testMultiVideoCapture();
    // testOpencvDnn();
    // testParseGps();

    int advice = HttpAdvice::SyncAdvice | HttpAdvice::NewConnectionAdvice | HttpAdvice::BeginningAdvice;
    HttpAdvice::InitAdvice(advice);
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_WARNING);

    drogon::app().loadConfigFile("E:\\Code\\CppTide\\config.yaml");
    // drogon::app().getLoop()->runAfter(0.0, [] { signal(SIGINT, signalHandler); });
    drogon::app().run();

    return 0;
}