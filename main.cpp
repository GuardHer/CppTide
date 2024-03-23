#include "opencv2/opencv.hpp"
#include <boost/version.hpp>
#include <drogon/drogon.h>
#include <iostream>

#include "src/http/controllers/asyncVideoStream.hpp"
#include "src/serial/serialBase.hpp"

void signalHandler()
{
    asyncVideoStream::closeCap();
    drogon::app().quit();
    system("exit");// 关闭命令行窗口
}

int main()
{
    // std::cout << FMT_VERSION << std::endl;
    std::cout << cv::getVersionString() << std::endl;

    std::cout << "Boost version: " << BOOST_VERSION / 100000 << "."
              << BOOST_VERSION / 100 % 1000 << "."
              << BOOST_VERSION % 100 << std::endl;

    drogon::app().loadConfigFile("E:\\Code\\v5lite\\vscoedTest\\config.json");
    drogon::app().addListener("0.0.0.0", 8000);
    drogon::app().setThreadNum(3);
    drogon::app().setLogLevel(trantor::Logger::kDebug);
    drogon::app().setIntSignalHandler(signalHandler);
    drogon::app().run();
    drogon::app().getLoop()->getEventLoopOfCurrentThread();
    drogon::app().getIOLoop(1);

    // std::cin.get();
    return 0;
}