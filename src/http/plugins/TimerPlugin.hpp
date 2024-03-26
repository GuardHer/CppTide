/**
 *
 *  cpptide_http_plugin_TimerPlugin.h
 *
 */

#pragma once

#include <drogon/plugins/Plugin.h>

namespace cpptide::http::plugin
{

class TimerPlugin : public drogon::Plugin<TimerPlugin>
{
public:
    TimerPlugin() {}
    /// This method must be called by drogon to initialize and start the plugin.
    /// It must be implemented by the user.
    void initAndStart(const Json::Value &config) override;

    /// This method must be called by drogon to shutdown the plugin.
    /// It must be implemented by the user.
    void shutdown() override;

    void setTimerInterval(double interval) { interval_ = interval; }
    double getTimerInterval() const { return interval_; }

    void setUpdateBoatControlData(bool update) { updateBoatControlData_ = update; }
    bool getUpdateBoatControlData() const { return updateBoatControlData_; }

private:
    Json::Value config_;
    double interval_ { 0.0 };
    bool updateBoatControlData_ { false };
};

}// namespace cpptide::http::plugin
