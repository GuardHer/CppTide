/**
 *
 *  V5Lite_YOLOPlugin.cc
 *
 */

#include "src/http/plugins/YOLOPlugin.hpp"

using namespace drogon;

namespace cpptide::http::plugin
{


void YOLOPlugin::initAndStart(const Json::Value &config)
{
    /// Initialize and start the plugin
    LOG_INFO << "YOLOPlugin initAndStart";

    configJsonValue_ = config;

    v5config_.confThreshold = config["confThreshold"].asFloat();
    v5config_.nmsThreshold  = config["nmsThreshold"].asFloat();
    v5config_.objThreshold  = config["objThreshold"].asFloat();
    v5config_.model_path    = config["model_path"].asString();
    v5config_.classesFile   = config["classesFile"].asString();

    v5litePtr_ = std::make_shared<YOLO::V5Lite>(v5config_);
}

void YOLOPlugin::shutdown()
{
    /// Shutdown the plugin
    v5litePtr_.reset();
}

}// namespace cpptide::http::plugin
