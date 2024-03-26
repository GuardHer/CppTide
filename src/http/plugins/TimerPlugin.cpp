/**
 *
 *  cpptide_http_plugin_TimerPlugin.cc
 *
 */

#include "src/http/plugins/TimerPlugin.hpp"

using namespace drogon;
namespace cpptide::http::plugin
{


void TimerPlugin::initAndStart(const Json::Value &config)
{
    /// Initialize and start the plugin
    config_                = config;
    interval_              = config_["interval"].asDouble();
    updateBoatControlData_ = config_["updateBoatControlData"].asBool();
    LOG_INFO << "TimerPlugin::initAndStart() interval: " << interval_;
}

void TimerPlugin::shutdown()
{
    /// Shutdown the plugin
}

}// namespace cpptide::http::plugin
