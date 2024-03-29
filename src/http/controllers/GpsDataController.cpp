#include "src/http/controllers/GpsDataController.hpp"
#include "src/http/controllers/BoatController.hpp"
#include "src/http/models/Gps.hpp"

namespace cpptide::http::controller
{

void GpsDataController::getGpsData(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, size_t count) const
{
    Json::Value res;
    Json::Value body;
    Json::Reader reader;

    std::string bodyStr = req->bodyData();
    size_t query_count  = count;

    if (count <= 0 && bodyStr.empty()) {
        res["code"]    = k400BadRequest;
        res["message"] = "The number of data requested must be greater than 0 and body is empty";
        auto response  = drogon::HttpResponse::newHttpJsonResponse(res);
        callback(response);
        return;
    }

    if (count <= 0) {
        try {
            reader.parse(bodyStr, body);
            LOG_DEBUG << "bodyStr: " << bodyStr;
            LOG_DEBUG << "body.toStyledString: " << body.toStyledString();
            query_count = body["count"].asUInt();
        } catch (const std::exception &e) {
            res["code"]    = k400BadRequest;
            res["message"] = "The number of data requested must be greater than 0";
            auto response  = drogon::HttpResponse::newHttpJsonResponse(res);
            callback(response);
            return;
        }
    }

    try {
        auto clientPtr = drogon::app().getDbClient();
        drogon::orm::Mapper<drogon_model::ship::Gps> mp(clientPtr);
        size_t cnt = mp.count();
        Json::Value json;
        if (query_count > cnt) {
            query_count    = cnt;
            res["message"] = "The number of data is less than the requested number";
        }
        // Query the last [query_count] data according to the id in descending order.
        auto sensor = mp.orderBy(drogon_model::ship::Gps::Cols::_id, drogon::orm::SortOrder::DESC).limit(query_count).findAll();
        for (auto &s: sensor) {
            Json::Value item;
            item["create_time"]    = s.getValueOfCreateTime().toCustomedFormattedStringLocal("%Y-%m-%d %H:%M:%S", false);// 创建时间
            item["sj"]             = s.getValueOfSj();                                                                   // 时间
            item["wgs84_lng"]      = s.getValueOfWgs84Lng();                                                             // 经度
            item["wgs84_lat"]      = s.getValueOfWgs84Lat();                                                             // 纬度
            item["gcj02_lng"]      = s.getValueOfGcj02Lng();                                                             // 经度
            item["gcj02_lat"]      = s.getValueOfGcj02Lat();                                                             // 纬度
            item["bd09_lng"]       = s.getValueOfBd09Lng();                                                              // 经度
            item["bd09_lat"]       = s.getValueOfBd09Lat();                                                              // 纬度
            item["gps_qual"]       = s.getValueOfGpsQual();                                                              // 定位质量
            item["num_sats"]       = s.getValueOfNumSats();                                                              // 卫星数量
            item["altitude"]       = s.getValueOfAltitude();                                                             // 海拔
            item["horizontal_dil"] = s.getValueOfHorizontalDil();                                                        // 水平精度因子
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
    res["method"] = "get gpsdata";
    auto response = drogon::HttpResponse::newHttpJsonResponse(res);
    callback(response);
}

}// namespace cpptide::http::controller