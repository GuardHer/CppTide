/**
 *
 *  v5Lite_YOLOPlugin.h
 *
 */

#pragma once

#include "src/v5lite/v5Lite.hpp"
#include <drogon/plugins/Plugin.h>

namespace YOLO
{

class YOLOPlugin : public drogon::Plugin<YOLOPlugin>
{
public:
    YOLOPlugin() {}
    /// This method must be called by drogon to initialize and start the plugin.
    /// It must be implemented by the user.
    void initAndStart(const Json::Value &config) override;

    /// This method must be called by drogon to shutdown the plugin.
    /// It must be implemented by the user.
    void shutdown() override;

    Net_config getV5Config() const { return v5config_; }


private:
    Json::Value config_json_;
    Net_config v5config_;
};

}// namespace YOLO
