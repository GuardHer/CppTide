#pragma once

#include <json/json.h>
#include <string>

namespace cpptide::common
{

/// @brief 将GPS数据解析为Json格式
/// @param data GPS数据
/// @return Json::Value Json格式的GPS数据
Json::Value parseGPSToJson(const std::string &data);

namespace GpsTransform
{

/// @brief GPS坐标结构体
typedef struct _POSITION
{
    double longitude;// 经度
    double latitude; // 纬度
} POSITION;

/// @brief 将度分秒格式的GPS坐标转换为十进制格式
std::string dmsToDecimal(const std::string &dms);

/// @brief 判断是否在中国
bool outof_China(double lon, double lat);

/// @brief 将经度转换为火星坐标系
double translate_lon(double lon, double lat);

/// @brief 将纬度转换为火星坐标系
double translate_lat(double lon, double lat);

/// @brief 将WGS84坐标转换为GCJ02坐标
POSITION wgs84ToGcj02(double wgs_lon, double wgs_lat);

/// @brief 将WGS84坐标转换为BD09坐标
POSITION wgs84Tobd09(double wgs_lon, double wgs_lat);

/// @brief 将GCJ02坐标转换为BD09坐标
POSITION gcj02Tobd09(double gcj_lon, double gcj_lat);

/// @brief 将GCJ02坐标转换为WGS84坐标
POSITION gcj02ToWgs84(double gcj_lon, double gcj_lat);

/// @brief 将BD09坐标转换为GCJ02坐标
POSITION bd09ToGcj02(double bd_lon, double bd_lat);


}// namespace GpsTransform


}// namespace cpptide::common