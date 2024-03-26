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
    int16_t Pump         = 0; // 水泵
    int16_t FindFish     = 0; // 寻鱼
};

class BoatController
{
public:
    /// @brief Boat direction
    enum class Direction
    {
        FORWARD, // 向前
        BACKWARD,// 向后
        LEFT,    // 向左
        RIGHT,   // 向右
        STOP,    // 停止
        UKNOWN   // 未知
    };

    enum class Device
    {
        LIGHT, // 灯光
        PUMP,  // 水泵
        UNKNOWN// 未知
    };

    /// @brief Device state
    enum class DeviceState
    {
        ON,    // 开
        OFF,   // 关
        TOGGLE,// 切换
        UNKNOWN// 未知
    };


    /// @brief Convert boat direction to string
    /// @param direction Boat state
    /// @return std::string
    static std::string directionToString(Direction direction);

    /// @brief Convert string to boat direction
    /// @param direction  String
    /// @return  Direction
    static Direction stringToDirection(const std::string &direction);

    /// @brief Convert device state to string
    /// @param state Device state
    /// @return std::string
    static std::string deviceStateToString(DeviceState state);

    /// @brief Convert string to  device state
    /// @param direction string
    /// @return Device state
    static DeviceState stringToDeviceState(const std::string &state);

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

    /// @brief Update control data to database, this is a scheduled task
    static void updateControlDataToDatabase();

public:
    static ControlData controlData_;
    static std::shared_ptr<serial::AsyncSerial> serialPtr_;
};

}// namespace cpptide::http::controller
