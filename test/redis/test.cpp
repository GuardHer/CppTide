#include <iostream>
#include <sw/redis++/redis++.h>
using namespace sw::redis;


/// std::pair<double, double>: georadius

template<class T>
void printContainer(const std::string &cmd, const T &container)
{
    std::cout << cmd << ": ";
    for (const auto &elem: container) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

int main()
{
    // 创建Redis连接对象
    ConnectionOptions connectionOptions;
    connectionOptions.host     = "139.224.250.45";
    connectionOptions.port     = 6379;
    connectionOptions.password = "tang044438";// 如果设置了密码，替换为实际密码

    Redis redis(connectionOptions);

    /// string
    redis.set("name", "zhangsan");           // 设置键值对
    redis.expire("name", 10);                // 设置过期时间为10秒
    std::vector<std::string> res;            // 保存所有key
    redis.keys("*", std::back_inserter(res));// 获取所有key
    printContainer("keys", res);             // 打印所有key
    redis.setex("name", 10, "zhangsan");     // 设置键值对并设置过期时间为10秒
    redis.setnx("name", "lisi");             // 设置键值对，如果键已存在则不设置
    auto count = redis.exists("name");       // 键存在的数量

    /// list 使用双向链表实现，可以重复
    std::vector<std::string> data = { "333", "444", "555" };// 数据
    redis.lpush("k1", data.begin(), data.end());            // 从左边插入数据
    redis.rpush("k1", "666");                               // 从右边插入数据
    res.clear();                                            // 清空数据
    redis.lrange("k1", 0, -1, std::back_inserter(res));     // 获取所有数据
    printContainer("lpush", res);                           // 打印所有数据
    redis.lpop("k1");                                       // 从左边弹出数据
    redis.rpop("k1");                                       // 从右边弹出数据
    auto len = redis.llen("k1");                            // 获取列表长度
    redis.ltrim("k1", 0, 3);                                // 截取列表,只保留0-3的数据
    res.clear();                                            // 清空数据
    redis.lrange("k1", 0, -1, back_inserter(res));          // 获取所有数据
    printContainer("ltrim", res);                           // 打印所有数据

    /// set 不能重复
    std::vector<std::string> data_set = { "333", "444", "555" };// 数据
    redis.sadd("k2", data_set.begin(), data_set.end());         // 添加数据
    res.clear();                                                // 清空数据
    redis.smembers("k2", back_inserter(res));                   // 获取所有数据
    printContainer("smembers", res);                            // 打印所有数据
    redis.sismember("k2", "333");                               // 判断是否存在
    redis.srem("k2", "333");                                    // 删除数据

    /// zset 有序集合
    std::vector<std::pair<std::string, double>> data_zset = { { "333", 3.3 }, { "444", 4.4 }, { "555", 5.5 } };// 数据
    redis.zadd("k3", data_zset.begin(), data_zset.end());                                                      // 添加数据
    res.clear();                                                                                               // 清空数据
    redis.zrange("k3", 0, -1, back_inserter(res));                                                             // 获取所有数据
    redis.zscore("k3", "333");                                                                                 // 获取分数
    printContainer("zrange", res);                                                                             // 打印所有数据

    /// hash
    std::vector<std::pair<std::string, std::string>> data_hash = { { "333", "3.3" }, { "444", "4.4" }, { "555", "5.5" } };// 数据
    redis.hset("k4", data_hash.begin(), data_hash.end());                                                                 // 添加数据
    redis.hset("k4", "666", "6.6");

    /// 发布订阅
    redis.publish("channel", "hello world");// 发布消息
    auto sub = redis.subscriber();          // 创建订阅对象
    sub.subscribe("channel");               // 订阅频道
    sub.on_message([](StringView channel, StringView msg) {
        std::cout << "channel: " << channel << ", msg: " << msg << std::endl;
    });
    sub.on_pmessage([](StringView pattern, StringView channel, StringView msg) {
        std::cout << "pattern: " << pattern << ", channel: " << channel << ", msg: " << msg << std::endl;
    });
    // sub.consume();// 消费消息, 阻塞

    /// xadd: 添加消息到流
    redis.xadd("stream", "*", { "name", "zhangsan" });
    redis.xadd("stream", "*", { "name", "lisi" });
    std::vector<std::pair<std::string, Optional<std::vector<std::pair<std::string, std::string>>>>> result;
    redis.xrange("key", "-", "+", std::back_inserter(result));
    redis.xlen("stream");

    /// geoadd: 添加地理位置, beijing, shanghai, guangzhou
    std::vector<std::tuple<StringView, double, double>> data_geo = {
        { "beijing", 116.405285, 39.904989 },
        { "shanghai", 121.472644, 31.231706 },
        { "guangzhou", 113.280637, 23.125178 }
    };
    redis.geoadd("city", data_geo.begin(), data_geo.end());
    auto re = redis.geopos("city", "guangzhou");
    std::cout << "geopos: " << re.value().first << " " << re.value().second << std::endl;
    auto dic = redis.geodist("city", "beijing", "shanghai", GeoUnit::KM);
    std::cout << dic.value() << std::endl;

    /// multi 事务
    auto tran = redis.transaction();
    tran.set("111", "222");
    tran.exec();

    return 0;
}