#include "src/serial/SerialCallback.hpp"
#include "src/common/common.hpp"
#include "src/http/models/Gps.hpp"
#include "src/http/models/Sensor.hpp"
#include "src/http/plugins/GpsSerialPlugin.hpp"
#include "src/http/plugins/SensorSerialPlugin.hpp"

#include <drogon/drogon.h>
#include <json/json.h>
#include <trantor/utils/Logger.h>

namespace cpptide::serial
{

/// type: Int, Float, Double, String, Bool ...
#define SET_PROPERTY(root, field, setterFunc, type)           \
    do {                                                      \
        try {                                                 \
            auto value = root[#field].as##type();             \
            setterFunc(value);                                \
        } catch (...) {                                       \
            LOG_WARN << "Failed to set " #field " property."; \
            setterFunc##ToNull();                             \
        }                                                     \
    } while (0)


/// sensor serial read callback
void SerialCallback::sensorReadCallback(const char *data, size_t size)
{
    LOG_INFO << "Read data size: " << size;
    // data to json
    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(data, root)) {
        LOG_ERROR << "Read data error!";
        return;
    }
    LOG_DEBUG << "Read data: " << root.toStyledString();


    if (!drogon::app().getPlugin<cpptide::http::plugin::SensorSerialPlugin>()->isSaveData()) {
        // not save data
        return;
    }

    // save data
    auto clientPtr = drogon::app().getDbClient();
    drogon::orm::Mapper<drogon_model::ship::Sensor> mp(clientPtr);
    drogon_model::ship::Sensor sensor;

    /// set sensor data
    SET_PROPERTY(root, Temperature, sensor.setTemperature, Float);// temperature
    SET_PROPERTY(root, DO, sensor.setDo, Float);                  // dissolved oxygen
    SET_PROPERTY(root, phValue, sensor.setOxygen, Float);         // phValue
    SET_PROPERTY(root, turbidity, sensor.setTuValue, Float);      // turbidity
    mp.insert(sensor);
}

/// sensor serial write complete callback
void SerialCallback::sensorWriteCompleteCallback(size_t size)
{
    LOG_DEBUG << "sensor Write data size: " << size;
}

/// gps serial read callback
void SerialCallback::gpsReadCallback(const char *data, size_t size)
{
    /// 遍历每一行
    std::string str(data, size);
    std::istringstream iss(std::move(str));
    std::string line;
    std::vector<drogon_model::ship::Gps> gpsVec;
    while (std::getline(iss, line)) {
        // check is gps data
        if (line.find("$GNGGA") == std::string::npos) {
            continue;
        }
        // remove '\n'
        if (line.back() == '\n') {
            line.pop_back();
        }
        LOG_DEBUG << "Read data: " << line;

        /// check is save data
        if (!drogon::app().getPlugin<cpptide::http::plugin::GpsSerialPlugin>()->isSaveData()) {
            // not save data
            return;
        }
        /// parse gps data
        Json::Value val = common::parseGPSToJson(line);
        if (val.empty()) {
            LOG_ERROR << "Failed to parse gps data!";
            continue;
        }
        drogon_model::ship::Gps gps;
        SET_PROPERTY(val, altitude, gps.setAltitude, String);           // altitude
        SET_PROPERTY(val, bd09_lat, gps.setBd09Lat, String);            // bd09_lat
        SET_PROPERTY(val, bd09_lng, gps.setBd09Lng, String);            // bd09_lng
        SET_PROPERTY(val, gcj02_lat, gps.setGcj02Lat, String);          // gcj02_lat
        SET_PROPERTY(val, gcj02_lng, gps.setGcj02Lng, String);          // gcj02_lng
        SET_PROPERTY(val, wgs84_lat, gps.setWgs84Lat, String);          // wgs84_lat
        SET_PROPERTY(val, wgs84_lng, gps.setWgs84Lng, String);          // wgs84_lng
        SET_PROPERTY(val, horizontal_dil, gps.setHorizontalDil, String);// horizontal_dil
        SET_PROPERTY(val, num_sats, gps.setNumSats, String);            // num_sats
        SET_PROPERTY(val, gps_qual, gps.setGpsQual, String);            // gps_qual
        SET_PROPERTY(val, sj, gps.setSj, String);                       // sj

        gpsVec.push_back(gps);
    }

    /// save gps data to mysql database
    auto clientPtr = drogon::app().getDbClient();              // get database client
    drogon::orm::Mapper<drogon_model::ship::Gps> mp(clientPtr);// get mapper
    for (auto &gps: gpsVec) {                                  // save data
        mp.insertFuture(gps);                                  // insert data
    }
}

/// gps serial write complete callback
void SerialCallback::gpsWriteCompleteCallback(size_t size)
{
    LOG_DEBUG << "gps Write data size: " << size;
}

}// namespace cpptide::serial