#include "src/http/controllers/LogoutController.hpp"

using namespace cpptide::http::controller;

void LogoutController::logout(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const
{
    req->session()->erase("loggedIn");
    req->session()->erase("email");
    HttpResponsePtr resp = HttpResponse::newHttpViewResponse("LoginPage");
    callback(resp);
}
