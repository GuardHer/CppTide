#include "src/http/controllers/EmailCaptchaController.hpp"
#include "src/common/common.hpp"
#include "src/http/models/EmailCaptcha.hpp"
#include <regex>

using namespace cpptide::http::controller;

void EmailCaptchaController::emailCaptcha(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, std::string email) const
{
    if (email.empty()) {
        HttpResponsePtr resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k400BadRequest);
        resp->setBody("Email is empty");
        callback(resp);
        return;
    }

    if (!std::regex_match(email, std::regex("^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\\.[a-zA-Z0-9-.]+$"))) {
        HttpResponsePtr resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k400BadRequest);
        resp->setBody("Email is invalid");
        callback(resp);
        return;
    }

    Json::Value json;
    json["email"] = email;

    // Generate a random number
    std::string captcha = common::generate_verification_code();
    // Save the captcha to the db
    auto dbPtr = drogon::app().getDbClient();
    drogon_model::ship::EmailCaptcha emailCaptcha;
    emailCaptcha.setEmail(email);
    emailCaptcha.setCaptcha(captcha);
    drogon::orm::Mapper<drogon_model::ship::EmailCaptcha> mapper(dbPtr);
    try {
        auto cri  = drogon::orm::Criteria(drogon_model::ship::EmailCaptcha::Cols::_email, drogon::orm::CompareOperator::EQ, email);
        auto code = mapper.findOne(cri);
        code.setCaptcha(captcha);
        mapper.update(code);
    } catch (const drogon::orm::DrogonDbException &e) {
        mapper.insert(emailCaptcha);
    }
    json["captcha"] = captcha;
    json["message"] = "Captcha has been sent to your email: " + captcha;

    auto resp = HttpResponse::newHttpJsonResponse(json);
    resp->addHeader("Access-Control-Allow-Origin", "*");

    callback(resp);
}
