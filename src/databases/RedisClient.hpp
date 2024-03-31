#pragma once

#include <sw/redis++/redis++.h>

namespace cpptide::database
{

class RedisClient
{

public:
    /// @brief Default constructor
    RedisClient() {};

    /// @brief Constructor
    /// @param connectionOptions Redis connection options
    explicit RedisClient(const sw::redis::ConnectionOptions &connectionOptions) {};

    /// @brief Constructor
    /// @param host
    /// @param password
    /// @param port default 6379
    explicit RedisClient(const std::string &host, const std::string &password, int port = 6379) {};
    ~RedisClient() = default;

public:
    /// @brief Connect to the Redis server
    /// @param connectionOptions Redis connection options
    void connect(const sw::redis::ConnectionOptions &connectionOptions) {};

    /// @brief Connect to the Redis server
    /// @param host
    /// @param password
    /// @param port default 6379
    void connect(const std::string &host, const std::string &password, int port = 6379) {};

    /// @brief Check if the connection is established
    /// @return bool
    bool isConnected() const { return isConnected_; }

    /// @brief Disconnect from the Redis server
    void disconnect() {};

    /// @brief Execute a Redis command
    /// @param fn
    void async(const std::function<void(sw::redis::Redis &)> &fn) {};

    /// @brief Get the connection options
    /// @return sw::redis::ConnectionOptions
    sw::redis::ConnectionOptions getConnectionOptions() const { return connectionOptions_; }

private:
    bool isConnected_ { false };
    sw::redis::ConnectionOptions connectionOptions_;
    std::shared_ptr<sw::redis::Redis> redisClientPtr_;
};

}// namespace cpptide::database