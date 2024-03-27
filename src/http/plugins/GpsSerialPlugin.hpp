/**
 *
 *  GpsSerialPlugin.h
 *
 */

#pragma once

#include "src/serial/AsyncSerial.hpp"
#include <drogon/plugins/Plugin.h>

namespace cpptide::http::plugin
{


class GpsSerialPlugin : public drogon::Plugin<GpsSerialPlugin>
{
public:
    GpsSerialPlugin() {}
    /// This method must be called by drogon to initialize and start the plugin.
    /// It must be implemented by the user.
    void initAndStart(const Json::Value &config) override;

    /// This method must be called by drogon to shutdown the plugin.
    /// It must be implemented by the user.
    void shutdown() override;

public:
    /// @brief Get the serial port
    std::shared_ptr<cpptide::serial::AsyncSerial> getAsyncSerialPtr() const { return asyncSerialPtr_; }

    /// @brief Get the serial port options
    /// @return SerialPortOptions
    serial::AsyncSerial::SerialPortOptions getOptions() const { return options_; }

    /// @brief Check if the serial port is open
    bool isOpen() const { return isOpen_; }

    /// @brief is save data
    /// @return bool isSaveData_
    bool isSaveData() const { return isSaveData_; }

private:
    std::shared_ptr<cpptide::serial::AsyncSerial> asyncSerialPtr_;// 串口指针
    serial::AsyncSerial::SerialPortOptions options_;              // 串口选项
    uint32_t readBufferSize_ { 1024 };                            // 读缓存大小
    bool isOpen_ { false };                                       // 是否打开串口
    bool isRead_ { false };                                       // 是否读取
    bool isWrite_ { false };                                      // 是否写入
    bool isSaveData_ { false };                                   // 是否保存数据
};

}// namespace cpptide::http::plugin