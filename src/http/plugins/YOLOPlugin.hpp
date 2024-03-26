/**
 *
 *  V5Lite_YOLOPlugin.h
 *
 */

#pragma once

#include "src/v5lite/V5Lite.hpp"
#include <drogon/plugins/Plugin.h>

namespace cpptide::http::plugin
{

class YOLOPlugin : public drogon::Plugin<YOLOPlugin>
{
public:
    using YOLOPluginPtr = std::shared_ptr<YOLOPlugin>;

public:
    YOLOPlugin() {}
    /// This method must be called by drogon to initialize and start the plugin.
    /// It must be implemented by the user.
    void initAndStart(const Json::Value &config) override;

    /// This method must be called by drogon to shutdown the plugin.
    /// It must be implemented by the user.
    void shutdown() override;

    YOLO::Net_config getV5Config() const { return v5config_; }

    std::shared_ptr<YOLO::V5Lite> getV5LitePtr() const { return v5litePtr_; }

private:
    std::shared_ptr<YOLO::V5Lite> v5litePtr_;

    Json::Value configJsonValue_;
    YOLO::Net_config v5config_;
};

}// namespace cpptide::http::plugin
