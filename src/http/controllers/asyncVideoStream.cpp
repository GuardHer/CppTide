#include "src/http/controllers/asyncVideoStream.hpp"

#include "src/http/plugins/OpencvPlugin.hpp"
#include "src/http/plugins/YOLOPlugin.hpp"

namespace cpptide::http::controller
{

YOLO::MultiVideoCapture asyncVideoStream::multi_cap_ = YOLO::MultiVideoCapture();
YOLO::v5Lite asyncVideoStream::net_                  = YOLO::v5Lite();
YOLO::Net_config asyncVideoStream::v5config_         = YOLO::Net_config();
int asyncVideoStream::current_index_                 = 0;

void asyncVideoStream::startVideoStream(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, int index) const
{
    if (multi_cap_.getCameraNum() == 0) {
        multi_cap_.init(drogon::app().getPlugin<http::plugin::OpencvPlugin>()->getCameraNum());
    }

    /// 打开摄像头
    if (!isOpened(index)) {
        multi_cap_.open(index);
        LOG_DEBUG << "open index: " << index;
    }

    /// 如果打开失败返回错误信息
    if (!isOpened(index)) {
        auto resp_err = drogon::HttpResponse::newHttpResponse();
        resp_err->setStatusCode(drogon::k500InternalServerError);
        resp_err->setBody("Failed to open video file");
        callback(resp_err);
        return;
    }

    current_index_ = index;

    /// 如果打开成功, 开始长传视频流
    auto resp = drogon::HttpResponse::newAsyncStreamResponse([](auto &&PH1) { return asyncVideoStream::VideoStream(std::forward<decltype(PH1)>(PH1)); });
    resp->addHeader("Content-Type", "multipart/x-mixed-replace;boundary=frame");
    callback(resp);
}

void asyncVideoStream::stopVideoStream(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) const
{

    /// 释放摄像头
    closeCap();

    /// 如果释放失败, 返回错误信息
    if (multi_cap_.isCloseAll()) {
        auto resp_err = drogon::HttpResponse::newHttpResponse();
        resp_err->setStatusCode(drogon::k500InternalServerError);
        resp_err->setBody("Failed to close video");
        callback(resp_err);
        return;
    }
    /// 释放成功, 返回成功信息
    auto resp_ok = drogon::HttpResponse::newHttpResponse();
    resp_ok->setStatusCode(drogon::k200OK);
    resp_ok->setBody("success to close video");
    callback(resp_ok);
}

void asyncVideoStream::VideoStream(drogon::ResponseStreamPtr resp)
{
    if (v5config_.classesFile.empty()) {
        v5config_ = drogon::app().getPlugin<http::plugin::YOLOPlugin>()->getV5Config();
    }

    if (!net_.isInit()) {
        net_.init(v5config_);
    }


    auto &cap_ = multi_cap_.get(current_index_);

    /// 如果没有打开摄像头, 返回
    if (!cap_.isOpened()) {
        LOG_ERROR << "video is not opened";
        return;
    }

    /// 打开成功则上传视频流
    while (cap_.isOpened()) {
        cv::Mat frame;
        cap_ >> frame;// Capture a frame
        if (!frame.empty()) {
            net_.detect(frame);
            std::vector<uchar> img_buffer;
            cv::imencode(".jpg", frame, img_buffer);
            std::string ima_data(img_buffer.begin(), img_buffer.end());
            auto img_size = img_buffer.size();
            std::ostringstream ss;
            ss << "--frame\r\nContent-Type: image/jpeg\r\n\r\n";
            ss << ima_data << "\r\n";
            resp->send(ss.str());

        } else {
            break;
        }
    }
    resp->close();
}

bool asyncVideoStream::isOpened(int index)
{
    return multi_cap_.isOpened(index);
}

void asyncVideoStream::closeCap()
{
    multi_cap_.closeAll();
}

}// namespace cpptide::http::controller