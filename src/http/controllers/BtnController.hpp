#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace cpptide::http::controller
{

class BtnController : public drogon::HttpController<BtnController>
{
public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(BtnController::get, "/{2}/{1}", Get); // path is /cpptide/http/controller/BtnController/{arg2}/{arg1}
    // METHOD_ADD(BtnController::your_method_name, "/{1}/{2}/list", Get); // path is /cpptide/http/controller/BtnController/{arg1}/{arg2}/list
    ADD_METHOD_TO(BtnController::getBtnStateData, "/cpptide/control/button/data", Get);                       // path is /absolute/path/{arg1}/{arg2}/list
    ADD_METHOD_TO(BtnController::changeBtnState, "/cpptide/control/button/change?target={1}&status={2}", Get);// path is /absolute/path/{arg1}/{arg2}/list

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    void getBtnStateData(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const;
    void changeBtnState(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, std::string target, int status) const;
};

}// namespace cpptide::http::controller
