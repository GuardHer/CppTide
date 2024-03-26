/**
 *
 *  SerialPlugin.hpp
 *
 */

#pragma once

#include "src/serial/AsyncSerial.hpp"
#include <drogon/plugins/Plugin.h>

namespace cpptide::http::plugin
{

class SerialPlugin : public drogon::Plugin<SerialPlugin>
{
public:
    using SerialPluginPtr = std::shared_ptr<SerialPlugin>;

public:
    SerialPlugin() {}
    /// This method must be called by drogon to initialize and start the plugin.
    /// It must be implemented by the user.
    void initAndStart(const Json::Value &config) override;

    /// This method must be called by drogon to shutdown the plugin.
    /// It must be implemented by the user.
    void shutdown() override;

    /// @brief Get the serial port
    std::shared_ptr<cpptide::serial::AsyncSerial> getAsyncSerialPtr() const { return asyncSerialPtr_; }

private:
    std::shared_ptr<cpptide::serial::AsyncSerial> asyncSerialPtr_;
};

}// namespace cpptide::http::plugin