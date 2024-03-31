#pragma once

#include <drogon/HttpSimpleController.h>

using namespace drogon;


namespace cpptide::http::controller
{

class TestCtrl : public drogon::HttpSimpleController<TestCtrl>
{
public:
    void asyncHandleHttpRequest(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) override;
    PATH_LIST_BEGIN
    // list path definitions here;
    PATH_ADD("/cpptide/sss", Get, Post);
    // PATH_ADD("/path", "filter1", "filter2", HttpMethod1, HttpMethod2...);
    PATH_LIST_END
};

}// namespace cpptide::http::controller
