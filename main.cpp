#include "src/http/advice/httpAdvice.hpp"
#include "src/serial/SerialBase.hpp"
#include "src/v5lite/MultiVideoCapture.hpp"
#include <boost/version.hpp>
#include <drogon/drogon.h>
#include <opencv2/core/utils/logger.hpp>

#include <fstream>
#include <iostream>
#include <onnxruntime_cxx_api.h>
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


int main()
{

    // testOpencv();
    // testMultiVideoCapture();
    // testDBModel();

    int advice = HttpAdvice::SyncAdvice | HttpAdvice::NewConnectionAdvice | HttpAdvice::BeginningAdvice;
    HttpAdvice::InitAdvice(advice);
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_WARNING);

    drogon::app().loadConfigFile("E:\\Code\\CppTide\\config.yaml");
    drogon::app().setLogLevel(trantor::Logger::kDebug);
    drogon::app().run();

    return 0;
}