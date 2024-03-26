#include "src/http/controllers/DeviceController.hpp"
#include "src/http/controllers/BoatController.hpp"

#include <trantor/utils/Date.h>

namespace cpptide::http::controller
{

void DeviceController::lightController(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, std::string direction) const
{
    BoatController::DeviceState deviceState = BoatController::stringToDeviceState(direction);
    ControlData &data                       = BoatController::controlData_;
    bool isErr                              = false;

    switch (deviceState) {
        case BoatController::DeviceState::ON:
            data.Light = 254;
            break;
        case BoatController::DeviceState::OFF:
            data.Light = 0;
            break;
        case BoatController::DeviceState::TOGGLE:
            data.Light = data.Light == 0 ? 254 : 0;
            break;
        default:
            isErr = true;
            break;
    }

    Json::Value respData;                                           // response Json data
    auto sendData = BoatController::controlData_;                   // control data
    BoatController::writeControlData(sendData);                     // write control data to serial port
    respData["data"] = BoatController::controlDataToJson(sendData); // response.data
    if (isErr) {                                                    // error
        respData["error"] = "Unknown light device state";           // response.error
        respData["code"]  = drogon::k500InternalServerError;        // response.code
    } else {                                                        // success
        respData["code"] = drogon::k200OK;                          // response.code
    }                                                               // if end
    auto resp = drogon::HttpResponse::newHttpJsonResponse(respData);// HttpResponse
    callback(resp);                                                 // callback
}

void DeviceController::pumpController(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback, std::string direction) const
{
    BoatController::DeviceState deviceState = BoatController::stringToDeviceState(direction);
    ControlData &data                       = BoatController::controlData_;
    bool isErr                              = false;

    switch (deviceState) {
        case BoatController::DeviceState::ON:
            data.Pump = 140;
            break;
        case BoatController::DeviceState::OFF:
            data.Pump = 0;
            break;
        case BoatController::DeviceState::TOGGLE:
            data.Light = data.Light == 0 ? 140 : 0;
            break;
        default:
            isErr = true;
            break;
    }

    Json::Value respData;                                           // response Json data
    auto sendData = BoatController::controlData_;                   // control data
    BoatController::writeControlData(sendData);                     // write control data to serial port
    respData["data"] = BoatController::controlDataToJson(sendData); // response.data
    if (isErr) {                                                    // error
        respData["error"] = "Unknown pump device state";            // response.error
        respData["code"]  = drogon::k500InternalServerError;        // response.code
    } else {                                                        // success
        respData["code"] = drogon::k200OK;                          // response.code
    }                                                               // if end
    auto resp = drogon::HttpResponse::newHttpJsonResponse(respData);// HttpResponse
    callback(resp);
}


}// namespace cpptide::http::controller