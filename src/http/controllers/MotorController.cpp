#include "src/http/controllers/MotorController.hpp"
#include "src/http/controllers/BoatController.hpp"

namespace cpptide::http::controller
{

void MotorController::directionController(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, std::string direction) const
{
    BoatController::Direction boatDirection = BoatController::stringToDirection(direction);
    ControlData &data                       = BoatController::controlData_;
    bool isErr                              = false;
    switch (boatDirection) {
        case BoatController::Direction::FORWARD:
            data.LeftMotor  = 70;
            data.RightMotor = 70;
            break;
        case BoatController::Direction::BACKWARD:
            data.LeftMotor  = 100;
            data.RightMotor = 100;
            break;
        case BoatController::Direction::LEFT:
            data.LeftMotor  = 100;
            data.RightMotor = 70;
            break;
        case BoatController::Direction::RIGHT:
            data.LeftMotor  = 70;
            data.RightMotor = 100;
            break;
        case BoatController::Direction::STOP:
            data.UpSteering   = 90;
            data.DownSteering = 90;
            break;
        default:
            isErr = true;
            break;
    }
    Json::Value respData;

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