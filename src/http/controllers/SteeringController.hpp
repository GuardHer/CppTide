#pragma once

#include <drogon/HttpController.h>
#include <json/json.h>


using namespace drogon;

namespace cpptide::http::controller
{
	
/// @brief Steering controller: 舵机, 控制摄像头的方向
class SteeringController : public drogon::HttpController<SteeringController>
{
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(SteeringController::directionController, "/cpptide/control/steering/{1}", drogon::Get);
    METHOD_LIST_END

public:
    /// @brief Control the direction of the creame (方向)
    /// @param req Request
    /// @param callback Callback function
    /// @param direction Direction of the boat
    void directionController(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, std::string direction) const;
};

}// namespace cpptide::http::controller