#pragma once

#include <drogon/HttpController.h>
#include <json/json.h>

using namespace drogon;

namespace cpptide::http::controller
{

class GpsDataController : public drogon::HttpController<GpsDataController>
{
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(GpsDataController::getGpsData, "/control/gps/data?count={1}", drogon::Get);
    METHOD_LIST_END

private:
    /// @brief Get gps data
    /// @param req Request
    /// @param callback Callback function
    void getGpsData(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, size_t count) const;
};

}// namespace cpptide::http::controller