/**
 *
 *  OpencvPlugin.cc
 *
 */

#include "src/http/plugins/OpencvPlugin.hpp"

#include <iostream>
#include <trantor/utils/Logger.h>

namespace cpptide::http::plugin
{

void OpencvPlugin::initAndStart(const Json::Value &config)
{
    config_json_ = config;
    /// Initialize and start the plugin
    LOG_INFO << "OpencvPlugin initAndStart";

    camera_num_ = config["camera_num"].asInt();
}

void OpencvPlugin::shutdown()
{
    /// Shutdown the plugin
}

Json::Value OpencvPlugin::getJsonConfig() const
{
    return config_json_;
}

}// namespace cpptide::http::plugin