/**
 *
 *  RedisPlugin.hpp
 *
 */

#pragma once

#include <drogon/plugins/Plugin.h>
#include <sw/redis++/redis++.h>
namespace cpptide::http::plugin
{

class RedisPlugin : public drogon::Plugin<RedisPlugin>
{
public:
    RedisPlugin() {}
    /// This method must be called by drogon to initialize and start the plugin.
    /// It must be implemented by the user.
    void initAndStart(const Json::Value &config) override;

    /// This method must be called by drogon to shutdown the plugin.
    /// It must be implemented by the user.
    void shutdown() override;

    /// @brief Get the Redis client pointer
    /// @return std::shared_ptr<sw::redis::Redis>
    std::shared_ptr<sw::redis::Redis> getRedisClientPtr() const { return redisClientPtr_; }

    /// @brief Get the Redis connection options
    /// @return sw::redis::ConnectionOptions
    sw::redis::ConnectionOptions getConnectionOptions() const { return connectionOptions_; }

    /// @brief Get the Redis port
    /// @return int
    int getPort() const { return port_; }

    /// @brief Get the Redis host
    /// @return std::string
    std::string getHost() const { return host_; }

    /// @brief Get the Redis password
    /// @return std::string
    std::string getPassword() const { return password_; }

private:
    sw::redis::ConnectionOptions connectionOptions_;
    std::shared_ptr<sw::redis::Redis> redisClientPtr_;
    int port_;
    std::string host_;
    std::string password_;
};

}// namespace cpptide::http::plugin
