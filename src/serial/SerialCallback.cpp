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

#define SET_SENSOR_PROPERTY(root, field, setterFunc)          \
    do {                                                      \
        try {                                                 \
            auto value = root[#field].asFloat();              \
            setterFunc(value);                                \
        } catch (...) {                                       \
            LOG_WARN << "Failed to set " #field " property."; \
        }                                                     \
    } while (0)

#define SET_GPS_PROPERTY(root, field, setterFunc)             \
    do {                                                      \
        try {                                                 \
            auto value = root[#field].asString();             \
            setterFunc(value);                                \
        } catch (...) {                                       \
            LOG_WARN << "Failed to set " #field " property."; \
        }                                                     \
    } while (0)


/// sensor串口读取回调函数
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

    SET_SENSOR_PROPERTY(root, Temperature, sensor.setTemperature);
    SET_SENSOR_PROPERTY(root, DO, sensor.setDo);
    SET_SENSOR_PROPERTY(root, phValue, sensor.setOxygen);
    SET_SENSOR_PROPERTY(root, turbidity, sensor.setTuValue);
    mp.insert(sensor);
}

/// sensor串口写入完成回调函数
void SerialCallback::sensorWriteCompleteCallback(size_t size)
{
    LOG_DEBUG << "sensor Write data size: " << size;
}

/// gps串口读取回调函数
void SerialCallback::gpsReadCallback(const char *data, size_t size)
{
    /// 遍历每一行
    std::string str(data, size);
    std::istringstream iss(std::move(str));
    std::string line;
    std::vector<drogon_model::ship::Gps> gpsVec;
    while (std::getline(iss, line)) {
        // 判断是不是以$GNGGA开头
        if (line.find("$GNGGA") == std::string::npos) {
            continue;
        }

        // 去掉line末尾的 \n 字符、
        if (line.back() == '\n') {
            line.pop_back();
        }
        LOG_DEBUG << "Read data: " << line;


        if (!drogon::app().getPlugin<cpptide::http::plugin::GpsSerialPlugin>()->isSaveData()) {
            // not save data
            return;
        }
        Json::Value val = common::parseGPSToJson(line);
        drogon_model::ship::Gps gps;
        SET_GPS_PROPERTY(val, altitude, gps.setAltitude);
        SET_GPS_PROPERTY(val, bd09_lat, gps.setBd09Lat);
        SET_GPS_PROPERTY(val, bd09_lng, gps.setBd09Lng);
        SET_GPS_PROPERTY(val, gcj02_lat, gps.setGcj02Lat);
        SET_GPS_PROPERTY(val, gcj02_lng, gps.setGcj02Lng);
        SET_GPS_PROPERTY(val, wgs84_lat, gps.setWgs84Lat);
        SET_GPS_PROPERTY(val, wgs84_lng, gps.setWgs84Lng);
        SET_GPS_PROPERTY(val, horizontal_dil, gps.setHorizontalDil);
        SET_GPS_PROPERTY(val, num_sats, gps.setNumSats);
        SET_GPS_PROPERTY(val, gps_qual, gps.setGpsQual);
        SET_GPS_PROPERTY(val, sj, gps.setSj);

        gpsVec.push_back(gps);
    }

    auto clientPtr = drogon::app().getDbClient();
    drogon::orm::Mapper<drogon_model::ship::Gps> mp(clientPtr);
    for (auto &gps: gpsVec) {
        mp.insertFuture(gps);
    }
}

/// gps串口写入完成回调函数
void SerialCallback::gpsWriteCompleteCallback(size_t size)
{
    LOG_DEBUG << "gps Write data size: " << size;
}

}// namespace cpptide::serial