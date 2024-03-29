#include "src/http/controllers/RegisterController.hpp"
#include "src/common/common.hpp"
#include "src/http/models/EmailCaptcha.hpp"
#include "src/http/models/User.hpp"

#include <drogon/utils/Utilities.h>
namespace cpptide::http::controller
{

void RegisterController::registerUser(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, std::string email, std::string captcha, std::string username, std::string password) const
{
    // Add your code here
    Json::Value json;
    HttpResponsePtr resp = HttpResponse::newHttpViewResponse("HomePage");

    auto dbPtr = drogon::app().getDbClient();
    drogon::orm::Mapper<drogon_model::ship::User> mapper(dbPtr);
    auto cri = drogon::orm::Criteria(drogon_model::ship::User::Cols::_email, drogon::orm::CompareOperator::EQ, email);
    try {
        // Check if the user already exists
        auto user     = mapper.findOne(cri);
        json["error"] = "User already exists";
        LOG_DEBUG << "User already exists";
    } catch (const drogon::orm::DrogonDbException &e) {
        // Check if the email is valid
        try {
            // find the email in the email_captcha table
            auto cri = drogon::orm::Criteria(drogon_model::ship::EmailCaptcha::Cols::_email, drogon::orm::CompareOperator::EQ, email);
            drogon::orm::Mapper<drogon_model::ship::EmailCaptcha> mapper_email(dbPtr);
            auto captcha_email = mapper_email.findOne(cri);
            if (captcha_email.getValueOfCaptcha() != captcha) {
                // captcha is invalid
                json["error"] = "Captcha is invalid, please resend the captcha";
                LOG_DEBUG << "Captcha is invalid";
                HttpResponsePtr resp_err = HttpResponse::newHttpViewResponse("RegisterPage");
                resp_err->setBody(json.toStyledString());
                callback(resp_err);
                return;
            }
            drogon_model::ship::User user;
            user.setEmail(email);
            user.setUsername(username);
            std::string salt = common::generateSalt();
            user.setSalt(salt);
            user.setPassword(drogon::utils::getMd5(password + salt));
            mapper.insert(user);
            json["email"]    = email;
            json["username"] = username;
            LOG_DEBUG << "User registered: " << email;

        } catch (const drogon::orm::DrogonDbException &e) {
            // not found
            json["error"] = "Email not found, please request a captcha first";
            LOG_DEBUG << "Email not found";
            HttpResponsePtr resp_err2 = HttpResponse::newHttpViewResponse("RegisterPage");
            resp_err2->setBody(json.toStyledString());
            callback(resp_err2);
            return;
        }
    }
    resp->setBody(json.toStyledString());
    callback(resp);
}

}// namespace cpptide::http::controller