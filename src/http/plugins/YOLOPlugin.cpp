/**
 *
 *  v5Lite_YOLOPlugin.cc
 *
 */

#include "src/http/plugins/YOLOPlugin.hpp"

using namespace drogon;
using namespace YOLO;

void YOLOPlugin::initAndStart(const Json::Value &config)
{
    /// Initialize and start the plugin
    LOG_INFO << "YOLOPlugin initAndStart";

    config_json_ = config;

    v5config_.confThreshold = config["confThreshold"].asFloat();
    v5config_.nmsThreshold  = config["nmsThreshold"].asFloat();
    v5config_.objThreshold  = config["objThreshold"].asFloat();
    v5config_.model_path    = config["model_path"].asString();
    v5config_.classesFile   = config["classesFile"].asString();
}

void YOLOPlugin::shutdown()
{
    /// Shutdown the plugin
}
