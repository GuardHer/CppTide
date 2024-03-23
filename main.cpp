#include "opencv2/opencv.hpp"
#include <boost/version.hpp>
#include <drogon/drogon.h>
#include <iostream>

#include "src/http/advice/httpAdvice.hpp"
#include "src/serial/serialBase.hpp"

int main()
{

    int advice = HttpAdvice::SyncAdvice | HttpAdvice::NewConnectionAdvice;
    HttpAdvice::InitAdvice(advice);
    drogon::app().loadConfigFile("../config.json");
    drogon::app().addListener("0.0.0.0", 8000);
    drogon::app().setThreadNum(3);
    drogon::app().setLogLevel(trantor::Logger::kDebug);
    drogon::app().run();

    // std::cin.get();
    return 0;
}