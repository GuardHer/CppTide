#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace cpptide::http::controller
{

class EmailCaptchaController : public drogon::HttpController<EmailCaptchaController>
{
public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(LogoutController::get, "/{2}/{1}", Get); // path is /cpptide/http/controller/LogoutController/{arg2}/{arg1}
    // METHOD_ADD(LogoutController::your_method_name, "/{1}/{2}/list", Get); // path is /cpptide/http/controller/LogoutController/{arg1}/{arg2}/list
    ADD_METHOD_TO(EmailCaptchaController::emailCaptcha, "/cpptide/email/captcha?email={1}", Get, Post);// path is /absolute/path/{arg1}/{arg2}/list

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    void emailCaptcha(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, std::string email) const;
};

}// namespace cpptide::http::controller
