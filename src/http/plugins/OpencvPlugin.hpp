/**
 *
 *  OpencvPlugin.h
 *
 */

#pragma once

#include <drogon/plugins/Plugin.h>

namespace YOLO
{

class OpencvPlugin : public drogon::Plugin<OpencvPlugin>
{
public:
    OpencvPlugin() {}
    /// This method must be called by drogon to initialize and start the plugin.
    /// It must be implemented by the user.
    void initAndStart(const Json::Value &config) override;

    /// This method must be called by drogon to shutdown the plugin.
    /// It must be implemented by the user.
    void shutdown() override;

    Json::Value getJsonConfig() const;

    int getCameraNum() const { return camera_num_; }

private:
    Json::Value config_json_;

    int camera_num_;
};

}// namespace YOLO