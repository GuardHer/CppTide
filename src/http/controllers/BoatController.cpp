#include "src/http/controllers/BoatController.hpp"
#include "src/http/plugins/SerialPlugin.hpp"

namespace cpptide::http::controller
{

ControlData BoatController::controlData_                        = ControlData();
std::shared_ptr<serial::AsyncSerial> BoatController::serialPtr_ = nullptr;

/////////////////////////////////////////////////////////////////////////////////////////
std::string BoatController::directionToString(Direction direction)
{
    switch (direction) {
        case Direction::FORWARD:
            return "forward";
        case Direction::BACKWARD:
            return "backward";
        case Direction::LEFT:
            return "left";
        case Direction::RIGHT:
            return "right";
        case Direction::STOP:
            return "stop";
        default:
            return "unknown";
    }
}

BoatController::Direction BoatController::stringToDirection(const std::string &direction)
{
    // 将字符串转换为小写形式
    std::string lowerDirection = direction;
    std::transform(lowerDirection.begin(), lowerDirection.end(), lowerDirection.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    // 比较转换后的字符串
    if (lowerDirection == "forward") {
        return Direction::FORWARD;
    } else if (lowerDirection == "backward") {
        return Direction::BACKWARD;
    } else if (lowerDirection == "left") {
        return Direction::LEFT;
    } else if (lowerDirection == "right") {
        return Direction::RIGHT;
    } else if (lowerDirection == "stop") {
        return Direction::STOP;
    } else {
        return Direction::UKNOWN;
    }
}

Json::Value BoatController::controlDataToJson(const ControlData &data)
{
    Json::Value json;
    json["LeftMotor"]    = std::to_string(data.LeftMotor);
    json["RightMotor"]   = std::to_string(data.RightMotor);
    json["UpSteering"]   = std::to_string(data.UpSteering);
    json["DownSteering"] = std::to_string(data.DownSteering);
    json["Light"]        = std::to_string(data.Light);
    json["FindFish"]     = std::to_string(data.FindFish);
    return json;
}

std::string BoatController::controlDataToString(const ControlData &data)
{
    return Json::FastWriter().write(controlDataToJson(data));
}

void BoatController::writeStringData(const std::string &data)
{
    serialPtr_->writeString(data);
}

void BoatController::writeControlData(const ControlData &data)
{
    Json::Value json = controlDataToJson(data);
    writeJsonData(json);
}

void BoatController::writeJsonData(const Json::Value &data)
{
    std::string str = data.toStyledString();
    writeStringData(str);
}


/////////////////// SteeringController ////////////////////
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


////////////////// MotorController ////////////////////
void MotorController::directionController(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, std::string direction) const
{
    BoatController::Direction boatDirection = BoatController::stringToDirection(direction);
    ControlData &data                       = BoatController::controlData_;
    Json::Value respData;
    bool isErr = false;
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