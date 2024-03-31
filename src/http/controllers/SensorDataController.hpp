#pragma once

#include <drogon/HttpController.h>
#include <json/json.h>


using namespace drogon;

namespace cpptide::http::controller
{

class SensorDataController : public drogon::HttpController<SensorDataController>
{
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(SensorDataController::getSensorData, "/cpptide/control/sensor/data?count={1}", drogon::Get);
    METHOD_LIST_END

private:
    /// @brief Get sensor data
    /// @param req Request
    /// @param callback Callback function
    void getSensorData(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, size_t count) const;
};

}// namespace cpptide::http::controller