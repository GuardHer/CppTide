#include "src/http/advice/httpAdvice.hpp"
#include "src/http/controllers/asyncVideoStream.hpp"

namespace cpptide::http::advice
{

void HttpAdvice::InitAdvice(int advice)
{
    if (advice & HttpAdvice::SyncAdvice) {
        LOG_INFO << "add SyncAdvice advice";
        drogon::app().registerSyncAdvice(HandleSyncAdvice);
    }
    if (advice & HttpAdvice::HttpResponseCreationAdvice) {
        LOG_INFO << "add HttpResponseCreationAdvice advice";
        drogon::app().registerHttpResponseCreationAdvice(HandleHttpResponseCreationAdvice);
    }
    if (advice & HttpAdvice::NewConnectionAdvice) {
        LOG_INFO << "add NewConnectionAdvice advice";
        drogon::app().registerNewConnectionAdvice(HandleNewConnectionAdvice);
    }
    if (advice & HttpAdvice::BeginningAdvice) {
        LOG_INFO << "add BeginningAdvice advice";
        drogon::app().registerBeginningAdvice(HandleBeginningAdvice);
    }
}

drogon::HttpResponsePtr HttpAdvice::HandleSyncAdvice(const drogon::HttpRequestPtr &req)
{
    LOG_INFO << "HandleSyncAdvice";
    http::controller::asyncVideoStream::closeCap();
    return drogon::HttpResponsePtr();
}

void HttpAdvice::HandleHttpResponseCreationAdvice(const drogon::HttpResponsePtr &resp)
{
}

bool HttpAdvice::HandleNewConnectionAdvice(const trantor::InetAddress &peer_addr, const trantor::InetAddress &local_addr)
{
    return true;
}

void HttpAdvice::HandleBeginningAdvice()
{
}

}