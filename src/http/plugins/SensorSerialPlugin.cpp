/**
 *
 *  SensorSerialPlugin.cpp
 *
 */

#include "src/http/plugins/SensorSerialPlugin.hpp"
#include "src/serial/SerialCallback.hpp"
#include <drogon/drogon.h>
#include <json/json.h>

using namespace drogon;
using namespace cpptide::serial;
using namespace cpptide::http::plugin;

void SensorSerialPlugin::initAndStart(const Json::Value &config)
{
    /// Initialize and start the plugin
    isOpen_         = config["is_open"].asBool();
    isRead_         = config["is_read"].asBool();
    isWrite_        = config["is_write"].asBool();
    isSaveData_     = config["is_save_data"].asBool();
    readBufferSize_ = config["read_buffer_size"].asUInt();

    /// if the serial port is not open, return directly
    if (!isOpen_) {
        LOG_INFO << "The Sensor serial port is not open";
        return;
    }

    /// get the serial port options
    auto port             = config["port"].asString();                                                          // 串口
    auto baudrate         = config["baudrate"].asUInt();                                                        // 波特率
    auto opt_csize        = config["opt_csize"].asUInt();                                                       // 数据位
    auto opt_parity       = static_cast<AsyncSerial::parity_t::type>(config["opt_parity"].asUInt());            // 奇偶校验
    auto opt_flow_control = static_cast<AsyncSerial::flow_control_t::type>(config["opt_flow_control"].asUInt());// 流控
    auto opt_stop_bits    = static_cast<AsyncSerial::stop_bits_t::type>(config["opt_stop_bits"].asUInt());      // 停止位

    /// set the serial port options
    options_.opt_csize        = AsyncSerial::character_size_t(opt_csize);
    options_.opt_parity       = AsyncSerial::parity_t(opt_parity);
    options_.opt_flow_control = AsyncSerial::flow_control_t(opt_flow_control);
    options_.opt_stop_bits    = AsyncSerial::stop_bits_t(opt_stop_bits);

    /// create the serial port
    asyncSerialPtr_ = std::make_shared<AsyncSerial>(port, baudrate, options_, readBufferSize_);
    asyncSerialPtr_->setReadCallback(std::bind(SerialCallback::sensorReadCallback, std::placeholders::_1, std::placeholders::_2));
    asyncSerialPtr_->setWriteCompleteCallback(std::bind(SerialCallback::sensorWriteCompleteCallback, std::placeholders::_1));

    LOG_INFO << "The Sensor serial port is open!"
             << " Port: " << port << " Baudrate: "
             << baudrate << " Data bits: " << opt_csize
             << " Parity: " << opt_parity << " Flow control: "
             << opt_flow_control << " Stop bits: " << opt_stop_bits;
}

void SensorSerialPlugin::shutdown()
{
    /// Shutdown the plugin
    asyncSerialPtr_.reset();
    LOG_DEBUG << "SensorSerialPlugin shutdown";
}
