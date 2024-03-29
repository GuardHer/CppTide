#include "src/common/common.hpp"

#include <chrono>
#include <random>
#include <trantor/utils/Logger.h>

namespace cpptide::common
{

static const double X_PI = 3.14159265358979323846 * 3000.0 / 180.0;// PI * 3000.0 / 180.0
static const double PI   = 3.1415926535897932384626;               // PI
static const double ee   = 0.00669342162296594323;                 // Eccentricity squared
static const double a    = 6378245.0;                              // WGS-84 major axis


Json::Value parseGPSToJson(const std::string &data)
{
    // data example: $GNGGA,121252.000,3937.3032,N,11611.6046,E,1,05,2.0,45.9,M,-5.7,M,,0000*77
    Json::Value root;

    std::istringstream iss(data);
    std::string token;
    std::vector<std::string> tokens;

    // Split the input string by comma
    while (std::getline(iss, token, ',')) {
        tokens.push_back(token);
    }

    // Check if the GPS data has at least 13 fields
    if (tokens.size() < 13) {
        LOG_ERROR << "Invalid GPS data: " << data;
        return Json::Value();// Return empty JSON object if data is invalid
    }

    // Create JSON object and populate it with GPS data
    GpsTransform::POSITION bd_pos  = GpsTransform::wgs84Tobd09(std::stod(tokens[4]), std::stod(tokens[2]));
    GpsTransform::POSITION gcj_pos = GpsTransform::wgs84ToGcj02(std::stod(tokens[4]), std::stod(tokens[2]));

    auto wgs84_lat = GpsTransform::dmsToDecimal(tokens[2]);
    auto wgs84_lng = GpsTransform::dmsToDecimal(tokens[4]);
    auto gcj02_lat = GpsTransform::dmsToDecimal(std::to_string(gcj_pos.latitude));
    auto gcj02_lng = GpsTransform::dmsToDecimal(std::to_string(gcj_pos.longitude));
    auto bd09_lat  = GpsTransform::dmsToDecimal(std::to_string(bd_pos.latitude));
    auto bd09_lng  = GpsTransform::dmsToDecimal(std::to_string(bd_pos.longitude));

    root["type"]           = tokens[0];// $GNGGA
    root["sj"]             = tokens[1];// time hhmmss.sss
    root["gps_qual"]       = tokens[6];// GPS status 0: invalid, 1: GPS, 2: DGPS
    root["num_sats"]       = tokens[7];// statellite number
    root["horizontal_dil"] = tokens[8];// horizontal accuracy
    root["altitude"]       = tokens[9];// altitude [m]
    root["wgs84_lat"]      = wgs84_lat;// wgs84 latitude
    root["wgs84_lng"]      = wgs84_lng;// wgs84 longitude
    root["gcj02_lat"]      = gcj02_lat;// gcj02 latitude
    root["gcj02_lng"]      = gcj02_lng;// gcj02 longitude
    root["bd09_lat"]       = bd09_lat; // bd09  latitude
    root["bd09_lng"]       = bd09_lng; // bd09  longitude

    return root;
}

std::string generateSalt() noexcept
{
    std::string salt;
    unsigned char result[MD5_DIGEST_LENGTH];
    char md5string[33];
    std::string str = std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
    MD5((unsigned char *) str.c_str(), str.length(), result);
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        // sprintf(&md5string[i * 2], "%02x", (unsigned int) result[i]);
        sprintf_s(&md5string[i * 2], 3, "%02x", (unsigned int) result[i]);
    }
    salt = md5string;
    return salt;
}

std::string generate_verification_code()
{
    // 创建随机数引擎
    std::random_device rd;
    std::mt19937 gen(rd());

    // 可以包含数字和字母的字符集
    const std::string charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const int charset_length  = static_cast<int>(charset.length());

    // 生成四位随机验证码
    std::string code;
    for (int i = 0; i < 4; ++i) {
        code.push_back(charset[gen() % charset_length]);
    }

    return code;
}

namespace GpsTransform
{

std::string dmsToDecimal(const std::string &dms)
{
    // Parse the resolution and points part from the string
    double data    = stod(dms);
    int degrees    = static_cast<int>(data / 100);
    int minutes    = static_cast<int>(data) % 100;
    double seconds = (data - degrees * 100 - minutes) * 60.0;

    // Convert the DMS to decimal degrees
    double decimalDegrees = degrees + minutes / 60.0 + seconds / 3600.0;
    return std::to_string(decimalDegrees);
}

bool outof_China(double lon, double lat)
{
    return (lon < 72.004 || lon > 137.8374 || lat < 0.8293 || lat > 55.8271 || false);
}

double translate_lon(double lon, double lat)
{
    double ret = 300.0 + lon + 2.0 * lat + 0.1 * lon * lon + 0.1 * lon * lat + 0.1 * sqrt(abs(lon));
    ret += (20.0 * sin(6.0 * lon * PI) + 20.0 * sin(2.0 * lon * PI)) * 2.0 / 3.0;
    ret += (20.0 * sin(lon * PI) + 40.0 * sin(lon / 3.0 * PI)) * 2.0 / 3.0;
    ret += (150 * sin(lon / 12.0 * PI) + 300.0 * sin(lon / 30.0 * PI)) * 2.0 / 3.0;
    return ret;
}

double translate_lat(double lon, double lat)
{
    double ret = -100 + 2.0 * lon + 3.0 * lat + 0.2 * lat * lat + 0.1 * lon * lat + 0.2 * sqrt((abs(lon)));
    ret += (20.0 * sin(6.0 * lon * PI) + 20 * sin(2.0 * lon * PI)) * 2.0 / 3.0;
    ret += (20.0 * sin(lat * PI) + 40.0 * sin(lat / 3.0 * PI)) * 2.0 / 3.0;
    ret += (160.0 * sin(lat / 12.0 * PI) + 320.0 * sin(lat / 30.0 * PI)) * 2.0 / 3.0;
    return ret;
}

POSITION wgs84ToGcj02(double wgs_lon, double wgs_lat)
{
    POSITION gcj_pos;
    if (outof_China(wgs_lon, wgs_lat)) {
        gcj_pos.longitude = wgs_lon;
        gcj_pos.latitude  = wgs_lat;
        return gcj_pos;
    } else {
        double dlat       = translate_lat(wgs_lon - 105.0, wgs_lat - 35.0);
        double dlon       = translate_lon(wgs_lon - 105.0, wgs_lat - 35.0);
        double radlat     = wgs_lat / 180.0 * PI;
        double magic      = sin(radlat);
        magic             = 1 - ee * magic * magic;
        double squrtmagic = sqrt(magic);
        dlon              = (dlon * 180.0) / (a / squrtmagic * cos(radlat) * PI);
        dlat              = (dlat * 180.0) / ((a * (1 - ee)) / (magic * squrtmagic) * PI);
        gcj_pos.longitude = wgs_lon + dlon;
        gcj_pos.latitude  = wgs_lat + dlat;
        return gcj_pos;
    }
}

POSITION wgs84Tobd09(double wgs_lon, double wgs_lat)
{
    POSITION gcj_pos = wgs84ToGcj02(wgs_lon, wgs_lat);
    return gcj02Tobd09(gcj_pos.longitude, gcj_pos.latitude);
}

POSITION gcj02Tobd09(double gcj_lon, double gcj_lat)
{
    POSITION bd_pos;
    double z         = sqrt(gcj_lon * gcj_lon + gcj_lat * gcj_lat) + 0.00002 * sin(gcj_lat * X_PI);
    double theta     = atan2(gcj_lat, gcj_lon) + 0.000003 * cos(gcj_lon * X_PI);
    bd_pos.longitude = z * cos(theta) + 0.0065;
    bd_pos.latitude  = z * sin(theta) + 0.006;
    return bd_pos;
}

POSITION gcj02ToWgs84(double gcj_lon, double gcj_lat)
{

    POSITION wgs_pos;
    if (outof_China(gcj_lon, gcj_lat)) {
        wgs_pos.longitude = gcj_lon;
        wgs_pos.latitude  = gcj_lat;
        return wgs_pos;
    } else {
        double dlat       = translate_lat(gcj_lon - 105.0, gcj_lat - 35.0);
        double dlon       = translate_lon(gcj_lon - 105.0, gcj_lat - 35.0);
        double radlat     = gcj_lat / 180.0 * PI;
        double magic      = sin(radlat);
        magic             = 1 - ee * magic * magic;
        double squrtmagic = sqrt(magic);
        dlon              = (dlon * 180.0) / (a / squrtmagic * cos(radlat) * PI);
        dlat              = (dlat * 180.0) / ((a * (1 - ee)) / (magic * squrtmagic) * PI);
        wgs_pos.longitude = gcj_lon - dlon;
        wgs_pos.latitude  = gcj_lat - dlat;
        return wgs_pos;
    }
}

POSITION bd09ToGcj02(double bd_lon, double bd_lat)
{
    POSITION gcj_pos;
    double x          = bd_lon - 0.0065;
    double y          = bd_lat - 0.006;
    double z          = sqrt(x * x + y * y) - 0.00002 * sin(y * X_PI);
    double theta      = atan2(y, x) - 0.000003 * cos(x * X_PI);
    gcj_pos.longitude = z * cos(theta);
    gcj_pos.latitude  = z * sin(theta);
    return gcj_pos;
}

}// namespace GpsTransform


}// namespace cpptide::common