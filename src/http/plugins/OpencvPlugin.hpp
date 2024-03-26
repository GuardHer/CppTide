/**
 *
 *  OpencvPlugin.h
 *
 */

#pragma once

#include "src/v5lite/MultiVideoCapture.hpp"
#include <drogon/plugins/Plugin.h>

namespace cpptide::http::plugin
{

class OpencvPlugin : public drogon::Plugin<OpencvPlugin>
{
public:
    using OpencvPluginPtr = std::shared_ptr<OpencvPlugin>;

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

    std::shared_ptr<YOLO::MultiVideoCapture> getMultiVideoCapturePtr() const { return multiVideoCapturePtr_; }

private:
    int camera_num_;
    std::shared_ptr<YOLO::MultiVideoCapture> multiVideoCapturePtr_;
    Json::Value configJsonValue_;
};

}// namespace cpptide::http::plugin
