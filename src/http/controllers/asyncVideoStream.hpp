#pragma once

#include "src/v5lite/multiVideoCapture.hpp"
#include "src/v5lite/v5Lite.hpp"
#include <drogon/HttpController.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace drogon;

class asyncVideoStream : public drogon::HttpController<asyncVideoStream>
{
public:
public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(asyncVideoStream::get, "/{2}/{1}", Get); // path is /asyncVideoStream/{arg2}/{arg1}
    // METHOD_ADD(asyncVideoStream::your_method_name, "/{1}/{2}/list", Get); // path is /asyncVideoStream/{arg1}/{arg2}/list
    ADD_METHOD_TO(asyncVideoStream::startVideoStream, "/start_video?index={1}", Get);// path is /absolute/path/{arg1}/{arg2}/list
    ADD_METHOD_TO(asyncVideoStream::stopVideoStream, "/stop_video", Get);            // path is /absolute/path/{arg1}/{arg2}/list

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    void startVideoStream(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, int index) const;
    void stopVideoStream(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const;


private:
    static void VideoStream(ResponseStreamPtr resp);

public:
    static void closeCap();
    [[nodiscard]] static bool isOpened(int index = 0);


private:
    static YOLO::Net_config v5config_;
    static std::shared_ptr<YOLO::v5Lite> net_;
    static std::shared_ptr<MultiVideoCapture> multi_cap_;
    static int current_index_;
};
