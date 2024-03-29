#include "src/http/controllers/LoginController.hpp"

namespace cpptide::http::controller
{

void LoginController::home(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const
{
    LOG_INFO << "Home page requested";
    bool loggedIn = req->session()->getOptional<bool>("loggedIn").value_or(false);
    HttpResponsePtr resp;
    if (loggedIn == false) {
        resp = HttpResponse::newHttpViewResponse("LoginPage");
    } else {
        resp = HttpResponse::newHttpViewResponse("LogoutPage");
    }
    callback(resp);
}

void LoginController::login(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const
{
    LOG_DEBUG << "Login requested";

    HttpResponsePtr resp = HttpResponse::newHttpResponse();
    LOG_DEBUG;
    std::optional<std::string> userOptional = req->getOptionalParameter<std::string>("user");
    std::string user                        = userOptional.value_or("");
    LOG_DEBUG;
    std::optional<std::string> passwdOptional = req->getOptionalParameter<std::string>("passwd");
    std::string passwd                        = passwdOptional.value_or("");
    LOG_INFO << "You have reached login, user: " << user << ", passwd: " << passwd << "";
    // NOTE: Do not use MD5 for the password hash under any
    // circumstances. We only use it because Drogon is not a
    // cryptography library, so it does not include a better hash
    // algorithm. Use Argon2 or BCrypt in a real product. username:
    // user, password: password123
    if (user == "user" && utils::getMd5("jadsjhdsajkh" + passwd) ==
                                  "5B5299CF4CEAE2D523315694B82573C9") {
        LOG_DEBUG;
        req->session()->insert("loggedIn", true);
        resp->setBody("<script>window.location.href = \"/\";</script>");
        callback(resp);
    } else {
        LOG_DEBUG;
        resp->setStatusCode(k401Unauthorized);
        resp->setBody("<script>window.location.href = \"/\";</script>");
        callback(resp);
    }
}

}// namespace cpptide::http::controller
