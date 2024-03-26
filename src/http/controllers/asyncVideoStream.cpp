#include "src/http/controllers/AsyncVideoStream.hpp"

namespace cpptide::http::controller
{


int AsyncVideoStream::current_index_                                             = 0;
std::shared_ptr<YOLO::V5Lite> AsyncVideoStream::yolov5Ptr_                       = nullptr;
std::shared_ptr<YOLO::MultiVideoCapture> AsyncVideoStream::multiVideoCapturePtr_ = nullptr;

void AsyncVideoStream::startVideoStream(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, int index) const
{
    /// 打开摄像头
    if (!isOpened(index)) {
        multiVideoCapturePtr_->open(index);
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
    auto resp = drogon::HttpResponse::newAsyncStreamResponse([](auto &&PH1) { return AsyncVideoStream::VideoStream(std::forward<decltype(PH1)>(PH1)); }, true);
    resp->addHeader("Content-Type", "multipart/x-mixed-replace;boundary=frame");
    callback(resp);
}

void AsyncVideoStream::stopVideoStream(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) const
{

    /// 释放摄像头
    closeCap();

    /// 如果释放失败, 返回错误信息
    if (multiVideoCapturePtr_->isCloseAll()) {
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

void AsyncVideoStream::VideoStream(drogon::ResponseStreamPtr resp)
{

    auto &cap_ = multiVideoCapturePtr_->get(current_index_);

    /// 如果没有打开摄像头, 返回
    if (!cap_.isOpened()) {
        LOG_ERROR << "video is not opened";
        return;
    }

    //320x240
    cap_.set(cv::CAP_PROP_FRAME_WIDTH, 320);
    cap_.set(cv::CAP_PROP_FRAME_HEIGHT, 240);

    /// 打开成功则上传视频流
    while (cap_.isOpened()) {
        cv::Mat frame;
        cap_ >> frame;// Capture a frame
        if (!frame.empty()) {
            yolov5Ptr_->detect(frame);
            std::vector<uchar> img_buffer;
            cv::imencode(".jpg", frame, img_buffer);
            std::string ima_data(img_buffer.begin(), img_buffer.end());
            std::ostringstream ss;
            ss << "--frame\r\nContent-Type: image/jpeg\r\n\r\n"
               << ima_data << "\r\n";
            resp->send(ss.str());
        } else {
            break;
        }
    }
    resp->close();
}

bool AsyncVideoStream::isOpened(int index)
{
    return multiVideoCapturePtr_->isOpened(index);
}

void AsyncVideoStream::closeCap()
{
    multiVideoCapturePtr_->closeAll();
}

}// namespace cpptide::http::controller