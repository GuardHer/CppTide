#include "src/http/controllers/SensorDataController.hpp"
#include "src/http/controllers/BoatController.hpp"
#include "src/http/models/Sensor.hpp"

namespace cpptide::http::controller
{

void SensorDataController::getSensorData(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, size_t count) const
{
    Json::Value res;

    try {
        auto clientPtr = drogon::app().getDbClient();
        drogon::orm::Mapper<drogon_model::ship::Sensor> mp(clientPtr);
        size_t cnt = mp.count();
        Json::Value json;
        size_t query_count = count;
        if (count > cnt) {
            query_count    = cnt;
            res["message"] = "The number of data is less than the requested number";
        }
        auto sensor = mp.orderBy(drogon_model::ship::Sensor::Cols::_id, drogon::orm::SortOrder::DESC).limit(query_count).findAll();
        for (auto &s: sensor) {
            Json::Value item;
            item["create_time"] = s.getValueOfCreateTime().toCustomedFormattedStringLocal("%Y-%m-%d %H:%M:%S", false);// 创建时间
            item["Temperature"] = s.getValueOfTemperature();                                                          // 温度
            item["TU_Value"]    = s.getValueOfTuValue();                                                              // 浊度
            item["Oxygen"]      = s.getValueOfOxygen();                                                               // 溶解氧
            item["DO"]          = s.getValueOfDo();                                                                   // 溶解氧饱和度
            json.append(item);
        }
        res["data"]  = json;
        res["code"]  = k200OK;
        res["count"] = query_count;
        if (query_count == count) {
            res["message"] = "success";
        }
    } catch (const drogon::orm::DrogonDbException &e) {
        res["code"]    = k500InternalServerError;
        res["message"] = e.base().what();
    }
    res["method"] = "get sensor data";
    auto response = drogon::HttpResponse::newHttpJsonResponse(res);
    callback(response);
}

}// namespace cpptide::http::controller