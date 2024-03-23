/**
 *
 *  testPlugin.h
 *
 */

#pragma once

#include <drogon/plugins/Plugin.h>


class testPlugin : public drogon::Plugin<testPlugin>
{
public:
    testPlugin() {}
    /// This method must be called by drogon to initialize and start the plugin.
    /// It must be implemented by the user.
    void initAndStart(const Json::Value &config) override;

    /// This method must be called by drogon to shutdown the plugin.
    /// It must be implemented by the user.
    void shutdown() override;
};
