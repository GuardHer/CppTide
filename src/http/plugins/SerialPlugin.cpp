/**
 *
 *  SerialPlugin.cpp
 *
 */

#include "src/http/plugins/SerialPlugin.hpp"
#include <drogon/drogon.h>

using namespace drogon;
using namespace cpptide::serial;
using namespace cpptide::http::plugin;

void SerialPlugin::initAndStart(const Json::Value &config)
{
    /// Initialize and start the plugin
    LOG_INFO << "SerialPlugin::initAndStart";
    cpptide::serial::AsyncSerial::SerialPortOptions options;
    auto port                = config["port"].asString();                                                          // 串口
    auto baudrate            = config["baudrate"].asUInt();                                                        // 波特率
    auto opt_csize           = config["opt_csize"].asUInt();                                                       // 数据位
    auto opt_parity          = static_cast<AsyncSerial::parity_t::type>(config["opt_parity"].asUInt());            // 奇偶校验
    auto opt_flow_control    = static_cast<AsyncSerial::flow_control_t::type>(config["opt_flow_control"].asUInt());// 流控
    auto opt_stop_bits       = static_cast<AsyncSerial::stop_bits_t::type>(config["opt_stop_bits"].asUInt());      // 停止位
    options.opt_csize        = AsyncSerial::character_size_t(opt_csize);
    options.opt_parity       = AsyncSerial::parity_t(opt_parity);
    options.opt_flow_control = AsyncSerial::flow_control_t(opt_flow_control);
    options.opt_stop_bits    = AsyncSerial::stop_bits_t(opt_stop_bits);

    asyncSerialPtr_ = std::make_shared<AsyncSerial>(port, baudrate, options);
}

void SerialPlugin::shutdown()
{
    /// Shutdown the plugin
    asyncSerialPtr_->close();
    asyncSerialPtr_.reset();
}
