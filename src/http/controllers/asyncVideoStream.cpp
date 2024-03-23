#include "src/http/controllers/asyncVideoStream.hpp"

#include <drogon/HttpAppFramework.h>
#include <drogon/drogon.h>


cv::VideoCapture asyncVideoStream::cap_;


void asyncVideoStream::startVideoStream(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const
{
    if (!isOpened()) {
        cap_.open(0);
    }
    if (!cap_.isOpened()) {
        auto resp_err = HttpResponse::newHttpResponse();
        resp_err->setStatusCode(k500InternalServerError);
        resp_err->setBody("Failed to open video file");
        callback(resp_err);
        return;
    }

    auto resp = HttpResponse::newAsyncStreamResponse(std::bind(asyncVideoStream::VideoStream, std::placeholders::_1), true);
    resp->setExpiredTime(1);
    resp->addHeader("Content-Type", "multipart/x-mixed-replace;boundary=frame");
    callback(resp);
}

void asyncVideoStream::stopVideoStream(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const
{
    if (isOpened()) {
        cap_.release();
    }
    if (isOpened()) {
        auto resp_err = HttpResponse::newHttpResponse();
        resp_err->setStatusCode(k500InternalServerError);
        resp_err->setBody("Failed to close video");
        callback(resp_err);
        return;
    }
    auto resp_ok = HttpResponse::newHttpResponse();
    resp_ok->setStatusCode(k200OK);
    resp_ok->setBody("success to close video");
    callback(resp_ok);
}

void asyncVideoStream::VideoStream(ResponseStreamPtr resp)
{

    if (!cap_.isOpened()) {
        LOG_ERROR << "failed video stream, cap is not open!";
        return;
    }
    while (cap_.isOpened()) {
        cv::Mat frame;
        cap_ >> frame;// Capture a frame
        if (!frame.empty()) {
            std::vector<uchar> img_buffer;
            cv::imencode(".jpg", frame, img_buffer);
            std::string ima_data(img_buffer.begin(), img_buffer.end());
            auto img_size = img_buffer.size();
            std::ostringstream ss;
            ss << "--frame\r\nContent-Type: image/jpeg\r\n\r\n";
            ss << ima_data << "\r\n";
            resp->send(ss.str());

            // std::this_thread::sleep_for(std::chrono::milliseconds(delay));// 10ms
        } else {
            break;
        }
    }
    resp->close();
}

bool asyncVideoStream::isOpened() const
{
    return asyncVideoStream::cap_.isOpened();
}

void asyncVideoStream::closeCap()
{
    if (cap_.isOpened()) {
        cap_.release();
    }
}
