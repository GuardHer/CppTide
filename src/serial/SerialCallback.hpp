#pragma once

#include "src/serial/AsyncSerial.hpp"

namespace cpptide::serial
{

class SerialCallback
{
public:
    /// @brief sensor串口读取回调函数
    static void sensorReadCallback(const char *data, size_t size);
    /// @brief sensor串口写入完成回调函数
    static void sensorWriteCompleteCallback(size_t size);
    /// @brief gps串口读取回调函数
    static void gpsReadCallback(const char *data, size_t size);
    /// @brief gps串口写入完成回调函数
    static void gpsWriteCompleteCallback(size_t size);

private:
    SerialCallback()  = delete;
    ~SerialCallback() = delete;
};

}// namespace cpptide::serial