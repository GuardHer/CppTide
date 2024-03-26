#include "src/http/controllers/SteeringController.hpp"
#include "src/http/controllers/BoatController.hpp"

namespace cpptide::http::controller
{

void SteeringController::directionController(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, std::string direction) const
{
    BoatController::Direction boatDirection = BoatController::stringToDirection(direction);
    ControlData &data                       = BoatController::controlData_;
    Json::Value respData;
    bool isErr = false;
    switch (boatDirection) {
        case BoatController::Direction::FORWARD:
            if (data.UpSteering >= 180) {
                data.UpSteering = 180;
            } else {
                data.UpSteering += 10;
            }
            break;
        case BoatController::Direction::BACKWARD:
            if (data.UpSteering <= 0) {
                data.UpSteering = 0;
            } else {
                data.UpSteering -= 10;
            }
            break;
        case BoatController::Direction::LEFT:
            if (data.DownSteering >= 180) {
                data.DownSteering = 180;
            } else {
                data.DownSteering += 10;
            }
            break;
        case BoatController::Direction::RIGHT:
            if (data.DownSteering <= 0) {
                data.DownSteering = 0;
            } else {
                data.DownSteering -= 10;
            }
            break;
        case BoatController::Direction::STOP:
            data.UpSteering   = 90;
            data.DownSteering = 90;
            break;
        default:
            isErr = true;
            break;
    }
    auto sendData = BoatController::controlData_;
    // 通过串口发送数据
    BoatController::writeControlData(sendData);

    // http response
    respData["data"] = BoatController::controlDataToJson(sendData);
    if (isErr) {
        respData["error"] = "Unknown direction";
        respData["code"]  = drogon::k500InternalServerError;
    } else {
        respData["code"] = drogon::k200OK;
    }
    auto resp = drogon::HttpResponse::newHttpJsonResponse(respData);
    callback(resp);
}

}// namespace cpptide::http::controller