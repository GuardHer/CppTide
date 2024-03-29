#pragma once

#include <json/json.h>
#include <openssl/md5.h>
#include <string>

namespace cpptide::common
{

/// @brief Parse GPS data into Json format
/// @param data GPS data string
/// @return Json::Value Json object
Json::Value parseGPSToJson(const std::string &data);

/// @brief Generate MD5 hash
/// @return std::string MD5 hash
std::string generateSalt() noexcept;

/// @brief Generate captcha
/// @return std::string Captcha
std::string generate_verification_code();

namespace GpsTransform
{

/// @brief GPS position structure
typedef struct _POSITION
{
    double longitude;// longitude
    double latitude; // latitude
} POSITION;

/// @brief Convert DMS to decimal
std::string dmsToDecimal(const std::string &dms);

/// @brief Check if the GPS data is out of China
bool outof_China(double lon, double lat);

/// @brief Translate the longitude to the Mars coordinate system
double translate_lon(double lon, double lat);

/// @brief Translate the latitude to the Mars coordinate system
double translate_lat(double lon, double lat);

/// @brief Convert WGS84 coordinates to GCJ02 coordinates
POSITION wgs84ToGcj02(double wgs_lon, double wgs_lat);

/// @brief Convert WGS84 coordinates to BD09 coordinates
POSITION wgs84Tobd09(double wgs_lon, double wgs_lat);

/// @brief Convert GCJ02 coordinates to BD09 coordinates
POSITION gcj02Tobd09(double gcj_lon, double gcj_lat);

/// @brief Convert BD09 coordinates to GCJ02 coordinates
POSITION gcj02ToWgs84(double gcj_lon, double gcj_lat);

/// @brief Convert BD09 coordinates to GCJ02 coordinates
POSITION bd09ToGcj02(double bd_lon, double bd_lat);


}// namespace GpsTransform


}// namespace cpptide::common