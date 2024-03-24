/**
 *
 *  Control.h
 *  DO NOT EDIT. This file is generated by drogon_ctl
 *
 */

#pragma once
#include <drogon/orm/Result.h>
#include <drogon/orm/Row.h>
#include <drogon/orm/Field.h>
#include <drogon/orm/SqlBinder.h>
#include <drogon/orm/Mapper.h>
#include <drogon/orm/BaseBuilder.h>
#ifdef __cpp_impl_coroutine
#include <drogon/orm/CoroMapper.h>
#endif
#include <trantor/utils/Date.h>
#include <trantor/utils/Logger.h>
#include <json/json.h>
#include <string>
#include <string_view>
#include <memory>
#include <vector>
#include <tuple>
#include <stdint.h>
#include <iostream>

namespace drogon
{
namespace orm
{
class DbClient;
using DbClientPtr = std::shared_ptr<DbClient>;
}
}
namespace drogon_model
{
namespace ship
{

class Control
{
  public:
    struct Cols
    {
        static const std::string _id;
        static const std::string _state;
        static const std::string _LeftMotor;
        static const std::string _RightMotor;
        static const std::string _UpServo;
        static const std::string _DownServo;
        static const std::string _Light;
        static const std::string _Pump;
        static const std::string _create_time;
        static const std::string _Oxygen_Pump;
        static const std::string _Feeding;
    };

    static const int primaryKeyNumber;
    static const std::string tableName;
    static const bool hasPrimaryKey;
    static const std::string primaryKeyName;
    using PrimaryKeyType = int32_t;
    const PrimaryKeyType &getPrimaryKey() const;

    /**
     * @brief constructor
     * @param r One row of records in the SQL query result.
     * @param indexOffset Set the offset to -1 to access all columns by column names,
     * otherwise access all columns by offsets.
     * @note If the SQL is not a style of 'select * from table_name ...' (select all
     * columns by an asterisk), please set the offset to -1.
     */
    explicit Control(const drogon::orm::Row &r, const ssize_t indexOffset = 0) noexcept;

    /**
     * @brief constructor
     * @param pJson The json object to construct a new instance.
     */
    explicit Control(const Json::Value &pJson) noexcept(false);

    /**
     * @brief constructor
     * @param pJson The json object to construct a new instance.
     * @param pMasqueradingVector The aliases of table columns.
     */
    Control(const Json::Value &pJson, const std::vector<std::string> &pMasqueradingVector) noexcept(false);

    Control() = default;

    void updateByJson(const Json::Value &pJson) noexcept(false);
    void updateByMasqueradedJson(const Json::Value &pJson,
                                 const std::vector<std::string> &pMasqueradingVector) noexcept(false);
    static bool validateJsonForCreation(const Json::Value &pJson, std::string &err);
    static bool validateMasqueradedJsonForCreation(const Json::Value &,
                                                const std::vector<std::string> &pMasqueradingVector,
                                                    std::string &err);
    static bool validateJsonForUpdate(const Json::Value &pJson, std::string &err);
    static bool validateMasqueradedJsonForUpdate(const Json::Value &,
                                          const std::vector<std::string> &pMasqueradingVector,
                                          std::string &err);
    static bool validJsonOfField(size_t index,
                          const std::string &fieldName,
                          const Json::Value &pJson,
                          std::string &err,
                          bool isForCreation);

    /**  For column id  */
    ///Get the value of the column id, returns the default value if the column is null
    const int32_t &getValueOfId() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t> &getId() const noexcept;
    ///Set the value of the column id
    void setId(const int32_t &pId) noexcept;

    /**  For column state  */
    ///Get the value of the column state, returns the default value if the column is null
    const int32_t &getValueOfState() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t> &getState() const noexcept;
    ///Set the value of the column state
    void setState(const int32_t &pState) noexcept;
    void setStateToNull() noexcept;

    /**  For column LeftMotor  */
    ///Get the value of the column LeftMotor, returns the default value if the column is null
    const int32_t &getValueOfLeftmotor() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t> &getLeftmotor() const noexcept;
    ///Set the value of the column LeftMotor
    void setLeftmotor(const int32_t &pLeftmotor) noexcept;
    void setLeftmotorToNull() noexcept;

    /**  For column RightMotor  */
    ///Get the value of the column RightMotor, returns the default value if the column is null
    const int32_t &getValueOfRightmotor() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t> &getRightmotor() const noexcept;
    ///Set the value of the column RightMotor
    void setRightmotor(const int32_t &pRightmotor) noexcept;
    void setRightmotorToNull() noexcept;

    /**  For column UpServo  */
    ///Get the value of the column UpServo, returns the default value if the column is null
    const int32_t &getValueOfUpservo() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t> &getUpservo() const noexcept;
    ///Set the value of the column UpServo
    void setUpservo(const int32_t &pUpservo) noexcept;
    void setUpservoToNull() noexcept;

    /**  For column DownServo  */
    ///Get the value of the column DownServo, returns the default value if the column is null
    const int32_t &getValueOfDownservo() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t> &getDownservo() const noexcept;
    ///Set the value of the column DownServo
    void setDownservo(const int32_t &pDownservo) noexcept;
    void setDownservoToNull() noexcept;

    /**  For column Light  */
    ///Get the value of the column Light, returns the default value if the column is null
    const int32_t &getValueOfLight() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t> &getLight() const noexcept;
    ///Set the value of the column Light
    void setLight(const int32_t &pLight) noexcept;
    void setLightToNull() noexcept;

    /**  For column Pump  */
    ///Get the value of the column Pump, returns the default value if the column is null
    const int32_t &getValueOfPump() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t> &getPump() const noexcept;
    ///Set the value of the column Pump
    void setPump(const int32_t &pPump) noexcept;
    void setPumpToNull() noexcept;

    /**  For column create_time  */
    ///Get the value of the column create_time, returns the default value if the column is null
    const ::trantor::Date &getValueOfCreateTime() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<::trantor::Date> &getCreateTime() const noexcept;
    ///Set the value of the column create_time
    void setCreateTime(const ::trantor::Date &pCreateTime) noexcept;
    void setCreateTimeToNull() noexcept;

    /**  For column Oxygen_Pump  */
    ///Get the value of the column Oxygen_Pump, returns the default value if the column is null
    const int32_t &getValueOfOxygenPump() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t> &getOxygenPump() const noexcept;
    ///Set the value of the column Oxygen_Pump
    void setOxygenPump(const int32_t &pOxygenPump) noexcept;
    void setOxygenPumpToNull() noexcept;

    /**  For column Feeding  */
    ///Get the value of the column Feeding, returns the default value if the column is null
    const int32_t &getValueOfFeeding() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t> &getFeeding() const noexcept;
    ///Set the value of the column Feeding
    void setFeeding(const int32_t &pFeeding) noexcept;
    void setFeedingToNull() noexcept;


    static size_t getColumnNumber() noexcept {  return 11;  }
    static const std::string &getColumnName(size_t index) noexcept(false);

    Json::Value toJson() const;
    Json::Value toMasqueradedJson(const std::vector<std::string> &pMasqueradingVector) const;
    /// Relationship interfaces
  private:
    friend drogon::orm::Mapper<Control>;
    friend drogon::orm::BaseBuilder<Control, true, true>;
    friend drogon::orm::BaseBuilder<Control, true, false>;
    friend drogon::orm::BaseBuilder<Control, false, true>;
    friend drogon::orm::BaseBuilder<Control, false, false>;
#ifdef __cpp_impl_coroutine
    friend drogon::orm::CoroMapper<Control>;
#endif
    static const std::vector<std::string> &insertColumns() noexcept;
    void outputArgs(drogon::orm::internal::SqlBinder &binder) const;
    const std::vector<std::string> updateColumns() const;
    void updateArgs(drogon::orm::internal::SqlBinder &binder) const;
    ///For mysql or sqlite3
    void updateId(const uint64_t id);
    std::shared_ptr<int32_t> id_;
    std::shared_ptr<int32_t> state_;
    std::shared_ptr<int32_t> leftmotor_;
    std::shared_ptr<int32_t> rightmotor_;
    std::shared_ptr<int32_t> upservo_;
    std::shared_ptr<int32_t> downservo_;
    std::shared_ptr<int32_t> light_;
    std::shared_ptr<int32_t> pump_;
    std::shared_ptr<::trantor::Date> createTime_;
    std::shared_ptr<int32_t> oxygenPump_;
    std::shared_ptr<int32_t> feeding_;
    struct MetaData
    {
        const std::string colName_;
        const std::string colType_;
        const std::string colDatabaseType_;
        const ssize_t colLength_;
        const bool isAutoVal_;
        const bool isPrimaryKey_;
        const bool notNull_;
    };
    static const std::vector<MetaData> metaData_;
    bool dirtyFlag_[11]={ false };
  public:
    static const std::string &sqlForFindingByPrimaryKey()
    {
        static const std::string sql="select * from " + tableName + " where id = ?";
        return sql;
    }

    static const std::string &sqlForDeletingByPrimaryKey()
    {
        static const std::string sql="delete from " + tableName + " where id = ?";
        return sql;
    }
    std::string sqlForInserting(bool &needSelection) const
    {
        std::string sql="insert into " + tableName + " (";
        size_t parametersCount = 0;
        needSelection = false;
            sql += "id,";
            ++parametersCount;
        if(dirtyFlag_[1])
        {
            sql += "state,";
            ++parametersCount;
        }
        if(dirtyFlag_[2])
        {
            sql += "LeftMotor,";
            ++parametersCount;
        }
        if(dirtyFlag_[3])
        {
            sql += "RightMotor,";
            ++parametersCount;
        }
        if(dirtyFlag_[4])
        {
            sql += "UpServo,";
            ++parametersCount;
        }
        if(dirtyFlag_[5])
        {
            sql += "DownServo,";
            ++parametersCount;
        }
        if(dirtyFlag_[6])
        {
            sql += "Light,";
            ++parametersCount;
        }
        if(dirtyFlag_[7])
        {
            sql += "Pump,";
            ++parametersCount;
        }
        sql += "create_time,";
        ++parametersCount;
        if(!dirtyFlag_[8])
        {
            needSelection=true;
        }
        if(dirtyFlag_[9])
        {
            sql += "Oxygen_Pump,";
            ++parametersCount;
        }
        if(dirtyFlag_[10])
        {
            sql += "Feeding,";
            ++parametersCount;
        }
        needSelection=true;
        if(parametersCount > 0)
        {
            sql[sql.length()-1]=')';
            sql += " values (";
        }
        else
            sql += ") values (";

        sql +="default,";
        if(dirtyFlag_[1])
        {
            sql.append("?,");

        }
        if(dirtyFlag_[2])
        {
            sql.append("?,");

        }
        if(dirtyFlag_[3])
        {
            sql.append("?,");

        }
        if(dirtyFlag_[4])
        {
            sql.append("?,");

        }
        if(dirtyFlag_[5])
        {
            sql.append("?,");

        }
        if(dirtyFlag_[6])
        {
            sql.append("?,");

        }
        if(dirtyFlag_[7])
        {
            sql.append("?,");

        }
        if(dirtyFlag_[8])
        {
            sql.append("?,");

        }
        else
        {
            sql +="default,";
        }
        if(dirtyFlag_[9])
        {
            sql.append("?,");

        }
        if(dirtyFlag_[10])
        {
            sql.append("?,");

        }
        if(parametersCount > 0)
        {
            sql.resize(sql.length() - 1);
        }
        sql.append(1, ')');
        LOG_TRACE << sql;
        return sql;
    }
};
} // namespace ship
} // namespace drogon_model