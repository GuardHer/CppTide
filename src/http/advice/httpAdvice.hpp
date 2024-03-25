#pragma once

#include <drogon/drogon.h>

namespace cpptide::http::advice
{

class HttpAdvice
{
public:
    enum AdviceType
    {
        SyncAdvice                 = 1,
        HttpResponseCreationAdvice = 2,
        NewConnectionAdvice        = 4,
        BeginningAdvice            = 8,
    };

public:
    static void InitAdvice(int advice);

private:
    /// @brief 该函数会在请求被创建后立即调用
    /// @param req 请求
    /// @return 如果通知函数返回一个非空的响应对象, 则该响应会被发送到客户端, 而不会调用实际的请求处理函数
    static drogon::HttpResponsePtr HandleSyncAdvice(const drogon::HttpRequestPtr &req);

    /// @brief 在收到请求之后, 请求响应之前执行
    /// @param resp 响应
    static void HandleHttpResponseCreationAdvice(const drogon::HttpResponsePtr &resp);

    /// @brief 当新连接建立时，该通知函数会立即被调用
    /// @param peer_addr 新连接的远程地址
    /// @param local_addr 新连接的本地地址
    /// @return 如果通知函数返回false值，则将关闭该连接。
    static bool HandleNewConnectionAdvice(const trantor::InetAddress &peer_addr, const trantor::InetAddress &local_addr);

    /// @brief 主事件循环运行后立即执行的开始通知
    static void HandleBeginningAdvice();
};

}// namespace cpptide::http::advice