#include "src/http/controllers/BtnController.hpp"
#include "src/http/models/BtnState.hpp"

namespace cpptide::http::controller
{


void BtnController::getBtnStateData(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) const
{
    // your code here
    Json::Value response;
    response["btnState"] = "on";
    try {
        auto dbClient = drogon::app().getDbClient();
        auto where    = drogon::orm::Criteria(drogon_model::ship::BtnState::Cols::_id, drogon::orm::CompareOperator::EQ, 1);
        drogon::orm::Mapper<drogon_model::ship::BtnState> mp(dbClient);
        auto btn            = mp.findOne(where);
        response["data"]    = btn.toJson();
        response["code"]    = k200OK;
        response["message"] = "OK";
    } catch (const std::exception &e) {
        response["code"]    = k500InternalServerError;
        response["message"] = e.what();
    }

    auto resp = HttpResponse::newHttpJsonResponse(response);
    callback(resp);
}

void BtnController::changeBtnState(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, std::string target, int status) const
{
    LOG_DEBUG << "target: " << target << ", status: " << status;
    drogon_model::ship::BtnState btn;
    Json::Value data, response;
    data[target]     = status;
    response["data"] = data;
    btn.setId(1);
    btn.updateByJson(data);

    try {
        auto dbClient = drogon::app().getDbClient();
        drogon::orm::Mapper<drogon_model::ship::BtnState> mp(dbClient);
        mp.update(btn);
        response["code"]    = k200OK;
        response["message"] = "OK";
    } catch (const std::exception &e) {
        response["code"]    = k500InternalServerError;
        response["message"] = e.what();
    }

    auto resp = HttpResponse::newHttpJsonResponse(response);
    callback(resp);
}

}// namespace cpptide::http::controller