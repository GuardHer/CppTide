#include "src/http/controllers/BoatController.hpp"
#include "src/http/models/Control.hpp"
#include "src/http/plugins/SerialPlugin.hpp"

namespace cpptide::http::controller
{

ControlData BoatController::controlData_                        = ControlData();
std::shared_ptr<serial::AsyncSerial> BoatController::serialPtr_ = nullptr;

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

std::string BoatController::deviceStateToString(DeviceState state)
{
    switch (state) {
        case DeviceState::ON:
            return "on";
        case DeviceState::OFF:
            return "off";
        case DeviceState::TOGGLE:
            return "toggle";
        default:
            return "unknown";
    }
}

BoatController::DeviceState BoatController::stringToDeviceState(const std::string &state)
{
    // 将字符串转换为小写形式
    std::string lowerState = state;
    std::transform(lowerState.begin(), lowerState.end(), lowerState.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    // 比较转换后的字符串
    if (lowerState == "on") {
        return DeviceState::ON;
    } else if (lowerState == "off") {
        return DeviceState::OFF;
    } else if (lowerState == "toggle") {
        return DeviceState::TOGGLE;
    } else {
        return DeviceState::UNKNOWN;
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
    json["Pump"]         = std::to_string(data.Pump);
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

void BoatController::updateControlDataToDatabase()
{
    auto clientPtr = drogon::app().getDbClient();
    drogon::orm::Mapper<drogon_model::ship::Control> mp(clientPtr);
    auto control = mp.findByPrimaryKey(1);//
    control.setLeftmotor(controlData_.LeftMotor);
    control.setRightmotor(controlData_.RightMotor);
    control.setUpservo(controlData_.UpSteering);
    control.setDownservo(controlData_.DownSteering);
    control.setLight(controlData_.Light);
    auto size = mp.update(control);

    LOG_INFO << "Update control data to database, size: " << size;
}

}// namespace cpptide::http::controller