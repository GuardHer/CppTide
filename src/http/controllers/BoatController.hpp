#pragma once

#include "src/serial/AsyncSerial.hpp"
#include <drogon/HttpController.h>
#include <json/json.h>

using namespace drogon;

namespace cpptide::http::controller
{

struct ControlData
{
    int16_t LeftMotor    = 90;// 左电机
    int16_t RightMotor   = 90;// 右电机
    int16_t UpSteering   = 90;// 上舵机
    int16_t DownSteering = 90;// 下舵机
    int16_t Light        = 0; // 灯光
    int16_t FindFish     = 0; // 寻鱼
};

class BoatController
{
public:
    /// @brief Boat state
    enum class Direction
    {
        FORWARD, // 向前
        BACKWARD,// 向后
        LEFT,    // 向左
        RIGHT,   // 向右
        STOP,    // 停止
        UKNOWN   // 未知
    };

    /// @brief Convert boat state to string
    /// @param direction Boat state
    /// @return std::string
    static std::string directionToString(Direction direction);

    /// @brief Convert string to boat state
    /// @param direction  String
    /// @return  Direction
    static Direction stringToDirection(const std::string &direction);

    /// @brief Convert control data to json
    /// @param data Control data
    /// @return Json::Value
    static Json::Value controlDataToJson(const ControlData &data);

    /// @brief Convert control data to string
    /// @param data Control data
    /// @return std::string
    static std::string controlDataToString(const ControlData &data);

    /// @brief Write string data to serial port
    /// @param data  String data
    static void writeStringData(const std::string &data);

    /// @brief Write control data to serial port
    /// @param data Control data
    static void writeControlData(const ControlData &data);

    /// @brief Write json data to serial port
    /// @param data Json data
    static void writeJsonData(const Json::Value &data);

public:
    static ControlData controlData_;
    static std::shared_ptr<serial::AsyncSerial> serialPtr_;
};

/// @brief Steering controller: 舵机, 控制摄像头的方向
class SteeringController : public drogon::HttpController<SteeringController>
{
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(SteeringController::directionController, "/control/steering/{1}", drogon::Get);
    METHOD_LIST_END

public:
    /// @brief Control the direction of the boat (方向)
    /// @param req Request
    /// @param callback Callback function
    /// @param direction Direction of the boat
    void directionController(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, std::string direction) const;
};

/// @brief Motor controller: 电机, 控制船的运动
class MotorController : public drogon::HttpController<MotorController>
{
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(SteeringController::directionController, "/control/motor/{1}", drogon::Get);
    METHOD_LIST_END

public:
    void directionController(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, std::string direction) const;
};

}// namespace cpptide::http::controller
