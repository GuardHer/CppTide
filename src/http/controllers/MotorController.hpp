#pragma once

#include <drogon/HttpController.h>
#include <json/json.h>

using namespace drogon;

namespace cpptide::http::controller
{

/// @brief Motor controller: 电机, 控制船的运动
class MotorController : public drogon::HttpController<MotorController>
{
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(MotorController::directionController, "/control/motor/{1}", drogon::Get);
    METHOD_LIST_END

public:
    /// @brief Control the direction of the boat (方向)
    /// @param req Request
    /// @param callback Callback function
    /// @param direction Direction of the boat
    void directionController(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, std::string direction) const;
};


}// namespace cpptide::http::controller