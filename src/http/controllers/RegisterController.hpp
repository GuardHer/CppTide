#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace cpptide::http::controller
{

class RegisterController : public drogon::HttpController<RegisterController>
{
public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(RegisterController::get, "/{2}/{1}", Get); // path is /cpptide/http/controller/RegisterController/{arg2}/{arg1}
    // METHOD_ADD(RegisterController::your_method_name, "/{1}/{2}/list", Get); // path is /cpptide/http/controller/RegisterController/{arg1}/{arg2}/list
    ADD_METHOD_TO(RegisterController::registerUser, "/register?email={1}&captcha={2}&username={3}&password={4}", Get, Post);// path is /absolute/path/{arg1}/{arg2}/list

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    void registerUser(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, std::string email, std::string captcha, std::string username, std::string password) const;
};

}// namespace cpptide::http::controller
