
#include <drogon/HttpController.h>
#include <json/json.h>

using namespace drogon;

namespace cpptide::http::controller
{

class DeviceController : public drogon::HttpController<DeviceController>
{
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(DeviceController::lightController, "/control/light/{1}", drogon::Get);
    ADD_METHOD_TO(DeviceController::pumpController, "/control/pump/{1}", drogon::Get);
    METHOD_LIST_END

private:
    /// @brief Control the light of the boat (灯光)
    /// @param req
    /// @param callback
    /// @param direction
    void lightController(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, std::string direction) const;

    /// @brief Control the light of the boat (灯光)
    /// @param req
    /// @param callback
    /// @param direction
    void pumpController(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, std::string direction) const;
};

}// namespace cpptide::http::controller