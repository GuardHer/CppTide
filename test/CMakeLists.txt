cmake_minimum_required(VERSION 3.10)
project(redisTest)

find_package(hiredis CONFIG REQUIRED)
find_package(redis++ CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)

add_executable(redisTest redis/test.cpp)
target_link_libraries(redisTest PRIVATE hiredis::hiredis)
target_link_libraries(redisTest PRIVATE redis++::redis++)
target_link_libraries(redisTest PRIVATE JsonCpp::JsonCpp)
