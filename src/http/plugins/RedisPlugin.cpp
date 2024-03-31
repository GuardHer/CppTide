/**
 *
 *  cpptide_http_plugin_RedisPlugin.cc
 *
 */

#include "src/http/plugins/RedisPlugin.hpp"
#include <json/json.h>

using namespace drogon;
using namespace sw;
using namespace cpptide::http::plugin;

void RedisPlugin::initAndStart(const Json::Value &config)
{
    /// Initialize and start the plugin
    port_     = config["port"].asInt();
    host_     = config["host"].asString();
    password_ = config["password"].asString();

    /// Create a Redis connection object
    connectionOptions_.host     = host_;
    connectionOptions_.port     = port_;
    connectionOptions_.password = password_;
    redisClientPtr_             = std::make_shared<sw::redis::Redis>(connectionOptions_);
}

void RedisPlugin::shutdown()
{
    if (redisClientPtr_) {
        redisClientPtr_.reset();
    }
}
