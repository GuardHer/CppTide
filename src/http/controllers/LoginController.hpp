#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace cpptide::http::controller
{

class LoginController : public drogon::HttpController<LoginController>
{
public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(LoginController::get, "/{2}/{1}", Get); // path is /cpptide/http/controller/LoginController/{arg2}/{arg1}
    // METHOD_ADD(LoginController::your_method_name, "/{1}/{2}/list", Get); // path is /cpptide/http/controller/LoginController/{arg1}/{arg2}/list
    ADD_METHOD_TO(LoginController::login, "/cpptide/login?email={1}&password={2}&username={3}", Get, Post);// path is /absolute/path/{arg1}/{arg2}/list
    // ADD_METHOD_TO(LoginController::login, "/login", Post);// path is /absolute/path/{arg1}/{arg2}/list
    ADD_METHOD_TO(LoginController::home, "/cpptide/home", Get);// path is /absolute/path/{arg1}/{arg2}/list

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    // void login(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, std::string email, std::string password) const;
    void login(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, std::string email, std::string password, std::string username) const;
    void home(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const;

public:
};

}// namespace cpptide::http::controller
