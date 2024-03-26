#include "src/http/advice/httpAdvice.hpp"
#include "src/http/controllers/AsyncVideoStream.hpp"
#include "src/http/controllers/BoatController.hpp"
#include "src/http/plugins/OpencvPlugin.hpp"
#include "src/http/plugins/SerialPlugin.hpp"
#include "src/http/plugins/TimerPlugin.hpp"
#include "src/http/plugins/YOLOPlugin.hpp"


namespace cpptide::http::advice
{

void HttpAdvice::InitAdvice(int advice)
{
    if (advice & HttpAdvice::SyncAdvice) {
        LOG_INFO << "add SyncAdvice advice";
        drogon::app().registerSyncAdvice(HandleSyncAdvice);
    }
    if (advice & HttpAdvice::HttpResponseCreationAdvice) {
        LOG_INFO << "add HttpResponseCreationAdvice advice";
        drogon::app().registerHttpResponseCreationAdvice(HandleHttpResponseCreationAdvice);
    }
    if (advice & HttpAdvice::NewConnectionAdvice) {
        LOG_INFO << "add NewConnectionAdvice advice";
        drogon::app().registerNewConnectionAdvice(HandleNewConnectionAdvice);
    }
    if (advice & HttpAdvice::BeginningAdvice) {
        LOG_INFO << "add BeginningAdvice advice";
        drogon::app().registerBeginningAdvice(HandleBeginningAdvice);
    }
}

drogon::HttpResponsePtr HttpAdvice::HandleSyncAdvice(const drogon::HttpRequestPtr &req)
{
    LOG_INFO << "HandleSyncAdvice";
    http::controller::AsyncVideoStream::closeCap();
    return drogon::HttpResponsePtr();
}

void HttpAdvice::HandleHttpResponseCreationAdvice(const drogon::HttpResponsePtr &resp)
{
}

bool HttpAdvice::HandleNewConnectionAdvice(const trantor::InetAddress &peer_addr, const trantor::InetAddress &local_addr)
{
    return true;
}

void HttpAdvice::HandleBeginningAdvice()
{
    LOG_INFO << "HandleBeginningAdvice";

    /// 通过插件获取指针
    controller::AsyncVideoStream::yolov5Ptr_            = drogon::app().getPlugin<plugin::YOLOPlugin>()->getV5LitePtr();
    controller::AsyncVideoStream::multiVideoCapturePtr_ = drogon::app().getPlugin<plugin::OpencvPlugin>()->getMultiVideoCapturePtr();
    controller::BoatController::serialPtr_              = drogon::app().getPlugin<plugin::SerialPlugin>()->getAsyncSerialPtr();

    /// 初始化定时任务
    // updateBoatControlData
    if (drogon::app().getPlugin<plugin::TimerPlugin>()->getUpdateBoatControlData()) {
        LOG_INFO << "start update BoatControlData, interval: " << drogon::app().getPlugin<plugin::TimerPlugin>()->getTimerInterval();
        drogon::app().getLoop()->runEvery(drogon::app().getPlugin<plugin::TimerPlugin>()->getTimerInterval(), []() {
            controller::BoatController::updateControlDataToDatabase();
        });
    } else {
        LOG_INFO << "update BoatControlData is not enabled";
    }
}

}// namespace cpptide::http::advice