#include "src/http/controllers/LoginController.hpp"
#include "src/common/common.hpp"
#include "src/http/models/User.hpp"

#include <drogon/utils/Utilities.h>

namespace cpptide::http::controller
{

void LoginController::home(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const
{
    LOG_INFO << "Home page requested";
    // bool loggedIn = req->session()->getOptional<bool>("loggedIn").value_or(false);
    // HttpResponsePtr resp;
    // if (loggedIn == false) {
    //     resp = HttpResponse::newHttpViewResponse("LoginPage");
    // } else {
    //     resp = HttpResponse::newHttpViewResponse("HomePage");
    // }

    HttpResponsePtr resp;
    HttpViewData data;
    data.insert("icon", "E:\\Code\\CppTide\\src\\http\\views\\static\\favicon.ico");

    resp = HttpResponse::newHttpViewResponse("HomePage.csp", data);
    resp->addHeader("Referrer-Policy", "strict-origin-when-cross-origin");

    resp->addHeader("Access-Control-Allow-Origin", "*");// CORS


    callback(resp);
}

void LoginController::login(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, std::string email, std::string password, std::string username) const
{
    if (req->method() == HttpMethod::Get) {
        HttpResponsePtr resp = HttpResponse::newHttpViewResponse("LoginPage");
        callback(resp);
        LOG_DEBUG << "GET request to login page";
        return;
    }

    Json::Value json;
    HttpResponsePtr resp = HttpResponse::newHttpViewResponse("HomePage");

    auto dbPtr = drogon::app().getDbClient();
    drogon::orm::Mapper<drogon_model::ship::User> mapper(dbPtr);
    auto cri = drogon::orm::Criteria(drogon_model::ship::User::Cols::_email, drogon::orm::CompareOperator::EQ, email);
    try {
        auto user = mapper.findOne(cri);
        // NOTE: Do not use MD5 for the password hash under any
        if (user.getValueOfPassword() == drogon::utils::getMd5(password + user.getValueOfSalt())) {
            req->session()->insert("loggedIn", true);
            req->session()->insert("email", user.getValueOfEmail());
            json["email"]    = user.getValueOfEmail();
            json["username"] = user.getValueOfUsername();
            LOG_DEBUG << "User logged in: " << user.getValueOfEmail();
        } else {
            // resp->setStatusCode(k401Unauthorized);
            json["error"] = "Invalid email or password";
            LOG_DEBUG << "Invalid email or password";
        }
    } catch (const drogon::orm::DrogonDbException &e) {
        // resp->setStatusCode(k500InternalServerError);
        json["error"] = "User not found, " + std::string(e.base().what());
        LOG_DEBUG << "User not found, " << e.base().what();
        HttpResponsePtr resp_err = HttpResponse::newHttpViewResponse("LoginPage");
        resp_err->setBody(json.toStyledString());
        callback(resp_err);
        return;
    }
    resp->setBody(json.toStyledString());
    callback(resp);
}

}// namespace cpptide::http::controller
