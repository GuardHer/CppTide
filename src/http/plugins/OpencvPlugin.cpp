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
    /// Initialize and start the plugin
    configJsonValue_ = config;
    LOG_INFO << "OpencvPlugin initAndStart";

    camera_num_ = config["camera_num"].asInt();

    multiVideoCapturePtr_ = std::make_shared<YOLO::MultiVideoCapture>(camera_num_);
}

void OpencvPlugin::shutdown()
{
    /// Shutdown the plugin
    multiVideoCapturePtr_->closeAll();
    multiVideoCapturePtr_.reset();
    LOG_DEBUG << "OpencvPlugin shutdown";
}

Json::Value OpencvPlugin::getJsonConfig() const
{
    return configJsonValue_;
}

}// namespace cpptide::http::plugin