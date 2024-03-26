#include "src/http/controllers/TestCtrl.hpp"
#include "src/http/models/Gps.hpp"
#include "src/http/models/User.hpp"

#include "src/http/plugins/OpencvPlugin.hpp"

namespace cpptide::http::controller
{

void TestCtrl::asyncHandleHttpRequest(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{

    using namespace drogon::orm;
    using namespace drogon_model::ship;

    try {
        auto clientPtr = drogon::app().getDbClient();
        Mapper<Gps> mp(clientPtr);
        // std::vector<Admin> uu = mp.orderBy(Admin::Cols::_id).limit(25).offset(0).findAll();
        auto iii = mp.count();
        auto uu  = mp.orderBy(Gps::Cols::_id).limit(5).offset(5).findAll();
    } catch (const DrogonDbException &e) {
        std::cout << "error:" << e.base().what() << std::endl;
    }

    Json::Value ret;
    ret["result"]    = "33";
    ret["user_name"] = "Jack";
    ret["gender"]    = 1;
    auto resp        = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

}// namespace cpptide::http::controller