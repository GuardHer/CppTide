#pragma once

#include "src/serial/AsyncSerial.hpp"

namespace cpptide::serial
{

class SerialCallback
{
public:
    /// @brief sensor Serial port reading callback function
    static void sensorReadCallback(const char *data, size_t size);
    /// @brief sensor Serial port write complete callback function
    static void sensorWriteCompleteCallback(size_t size);
    /// @brief gps Serial port reading callback function
    static void gpsReadCallback(const char *data, size_t size);
    /// @brief gps Serial port write complete callback function
    static void gpsWriteCompleteCallback(size_t size);

private:
    /// @brief Constructor and destructor are deleted
    SerialCallback()  = delete;
    ~SerialCallback() = delete;
};

}// namespace cpptide::serial