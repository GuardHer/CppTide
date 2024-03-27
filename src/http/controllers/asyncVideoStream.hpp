#pragma once

#include "src/v5lite/MultiVideoCapture.hpp"
#include "src/v5lite/V5Lite.hpp"
#include <drogon/HttpController.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include "src/http/plugins/OpencvPlugin.hpp"
#include "src/http/plugins/YOLOPlugin.hpp"

namespace cpptide::http::controller
{

class AsyncVideoStream : public drogon::HttpController<AsyncVideoStream>
{
public:
public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(AsyncVideoStream::get, "/{2}/{1}", Get); // path is /AsyncVideoStream/{arg2}/{arg1}
    // METHOD_ADD(AsyncVideoStream::your_method_name, "/{1}/{2}/list", Get); // path is /AsyncVideoStream/{arg1}/{arg2}/list
    ADD_METHOD_TO(AsyncVideoStream::startVideoStream, "/start_video?index={1}", drogon::Get);// path is /absolute/path/{arg1}/{arg2}/list
    ADD_METHOD_TO(AsyncVideoStream::stopVideoStream, "/stop_video", drogon::Get);            // path is /absolute/path/{arg1}/{arg2}/list

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    void startVideoStream(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, int index) const;
    void stopVideoStream(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) const;


private:
    static void VideoStream(drogon::ResponseStreamPtr resp);

public:
    static void closeCap();
    [[nodiscard]] static bool isOpened(int index = 0);


public:
    static int current_index_;
    static std::shared_ptr<YOLO::V5Lite> yolov5Ptr_;
    static std::shared_ptr<YOLO::MultiVideoCapture> multiVideoCapturePtr_;
};

}// namespace cpptide::http::controller
