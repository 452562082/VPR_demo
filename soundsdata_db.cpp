#include "SoundsData_db.h"
#include "utils/logger.h"
#include <QVariant>
#include <QVector>

namespace {
    const char* databaseName = "soundsdata";
//    const char* hostName = "127.0.0.1";
//    const int port = 3306;
    const char* username = "root";
    const char* password = "123456";
}

SoundsData_db* SoundsData_db::m_instance = nullptr;
SoundsData_db::SoundsData_db() : m_is_open(false),m_last_error(Error::SUCCESS)
{
}

SoundsData_db::~SoundsData_db()
{
    close();
    if (SoundsData_db::m_instance != nullptr) {
        delete SoundsData_db::m_instance;
        SoundsData_db::m_instance = nullptr;
    }
}

SoundsData_db* SoundsData_db::GetInstance()
{
    if(m_instance == nullptr){
        m_instance = new SoundsData_db();
    }
    return m_instance;
}

bool SoundsData_db::AddRegistrantInfo(const QString &spk_id, const QString &name, const QString &head_path)
{
    if(!m_is_open){
        if(!open()){
            return false;
        }
    }

    QString tablename = "registrantsinfo";
    QSqlQuery sql_query(m_database);

    QString sql_str = QString("INSERT INTO %1.%2 (Spk_id,Name,HeadPath,RegistrationTime) values(?,?,?,?);").arg(databaseName).arg(tablename);
    sql_query.prepare(sql_str);
    sql_query.addBindValue(spk_id);
    sql_query.addBindValue(name);
    sql_query.addBindValue(head_path);
    sql_query.addBindValue(QDateTime::currentDateTime());
    if(!sql_query.exec()){
        Logger::Error(QString("SQL - insert into Table '%1' error: %2").arg(tablename).arg(sql_query.lastError().text()));
        m_last_error = Error::INSERT_INTO_TABLE_ERROR;
        return false;
    }else{
        Logger::Info(QString("SQL insert into Table '%1' success.").arg(tablename));
        m_last_error = Error::SUCCESS;
        return true;
    }
}

bool SoundsData_db::GetAllRegistrantsInfo(QVector<RegistrantInfo> &ret)
{
    if(!m_is_open){
        if(!open()){
            return false;
        }
    }

    ret.clear();
    QString tablename = "registrantsinfo";
    QSqlQuery sql_query(m_database);
    QString sql_str = QString("SELECT * FROM %1.%2;").arg(databaseName).arg(tablename);
    if(!sql_query.exec(sql_str)){
        Logger::Error(QString("SQL - SELECT * FROM '%1' error.").arg(tablename));
        m_last_error = Error::SELECT_FROM_TABLE_ERROR;
        return false;
    }else{
        Logger::Info(QString("SQL - SELECT * FROM '%1' success.").arg(tablename));
        m_last_error = Error::SUCCESS;
        while(sql_query.next()){
            RegistrantInfo item;
            item.spk_id = sql_query.value("Spk_id").toString();
            item.name = sql_query.value("Name").toString();
            item.head_path = sql_query.value("HeadPath").toString();
            item.registration_time = sql_query.value("RegistrationTime").toDateTime();           
            ret.append(item);
        }
        return true;
    }
}

bool SoundsData_db::GetRegistrantInfoBySpkId(const QString &spk_id, RegistrantInfo &ret)
{
    if(!m_is_open){
        if(!open()){
            return false;
        }
    }

    QString tablename = "registrantsinfo";
    QSqlQuery sql_query(m_database);
    QString sql_str = QString("SELECT * FROM %1.%2 WHERE Spk_id = '%3';").arg(databaseName).arg(tablename).arg(spk_id);
    if(!sql_query.exec(sql_str)){
        Logger::Error(QString("SQL - SELECT FROM '%1' WHERE Spk_id = '%2' error.").arg(tablename).arg(spk_id));
        m_last_error = Error::SELECT_FROM_TABLE_ERROR;
        return false;
    }

    while(sql_query.next()){
        ret.spk_id = sql_query.value("Spk_id").toString();
        ret.name = sql_query.value("Name").toString();
        ret.head_path = sql_query.value("HeadPath").toString();
        ret.registration_time = sql_query.value("RegistrationTime").toDateTime();       
        break;
    }
    Logger::Info(QString("SQL - SELECT FROM '%1' WHERE Spk_id = '%2' success.").arg(tablename).arg(spk_id));
    m_last_error = Error::SUCCESS;
    return true;
}

bool SoundsData_db::DeleteRegistrantInfoBySpkId(const QString &spk_id)
{
    if(!m_is_open){
        if(!open()){
            return false;
        }
    }

    QString tablename = "registrantsinfo";
    QSqlQuery sql_query(m_database);
    QString sql_str = QString("DELETE FROM %1.%2 WHERE Spk_id = '%3';").arg(databaseName).arg(tablename).arg(spk_id);
    if(!sql_query.exec(sql_str)){
        Logger::Error(QString("SQL - DELETE FROM '%1' WHERE Spk_id = '%2' error.").arg(tablename).arg(spk_id));
        m_last_error = Error::DELETE_FROM_TABLE_ERROR;
        return false;
    }else{
        Logger::Info(QString("SQL - DELETE FROM '%1' WHERE Spk_id = '%2' success.").arg(tablename).arg(spk_id));
        m_last_error = Error::SUCCESS;
        return true;
    }
}

SoundsData_db::Error SoundsData_db::GetLastError() const
{
    return m_last_error;
}

void SoundsData_db::setHostName(const QString &hostname)
{
    m_hostName = hostname;
}

void SoundsData_db::setPort(int port)
{
    m_port = port;
}

bool SoundsData_db::open()
{
    m_database = QSqlDatabase::addDatabase("QMYSQL");
    m_database.setHostName(m_hostName);
    m_database.setPort(m_port);
    m_database.setUserName(username);
    m_database.setPassword(password);
    if(!m_database.open()){
        Logger::Error(QString("SQL - open error - %1").arg(m_database.lastError().text()));
        m_last_error = Error::DB_OPEN_ERROR;
        m_is_open = false;
        return false;
    }
    createDatabase();
    close();
    m_database.setDatabaseName(databaseName);
    if(!m_database.open()){
        Logger::Error(QString("SQL - open error - %1").arg(m_database.lastError().text()));
        m_last_error = Error::DB_OPEN_ERROR;
        m_is_open = false;
        return false;
    }
    createRegistrantsInfoTable();
    m_is_open = true;
    return true;
}

void SoundsData_db::close()
{
    m_database.close();
    m_is_open = false;
}

void SoundsData_db::createDatabase()
{
    QSqlQuery sql_query(m_database);
    QString sql_str = "show databases;";
    sql_query.exec(sql_str);
    bool is_database_exist = false;
    while(sql_query.next()){
        if(sql_query.value(0).toString().compare(databaseName) == 0){
            is_database_exist = true;
            break;
        }
    }

    if(is_database_exist){
        Logger::Info(QString("SQL - SCHEMA '%1' is existed.").arg(databaseName));
        m_last_error = Error::SUCCESS;
        return;
    }
    sql_str = QString("CREATE SCHEMA `%1` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;").arg(databaseName);
    if(!sql_query.exec(sql_str)){
        Logger::Error(QString("SQL - create SCHEMA '%1' error:").arg(databaseName).arg(sql_query.lastError().text()));
        m_last_error = Error::CREATE_SCHEMA_ERROR;
    }else{
        Logger::Info(QString("SQL - SCHEMA '%1' created.").arg(databaseName));
        m_last_error = Error::SUCCESS;
    }
}

void SoundsData_db::createRegistrantsInfoTable()
{
    QString tablename = "registrantsinfo";
    QSqlQuery sql_query(m_database);

    QString sql_str = "show tables;";
    bool is_table_exist = false;
    sql_query.exec(sql_str);
    while (sql_query.next()) {
        if(sql_query.value(0).toString().compare(tablename) == 0){
            is_table_exist = true;
            break;
        }
    }
    if(is_table_exist){
        Logger::Info(QString("SQL - Table '%1' is existed.").arg(tablename));
        m_last_error = Error::SUCCESS;
        return;
    }

    sql_str = QString("CREATE TABLE `%1`.`%2` (`Spk_id` VARCHAR(32) NOT NULL,`Name` VARCHAR(20) NOT NULL,`HeadPath` VARCHAR(260) NOT NULL,`RegistrationTime` DATETIME NOT NULL,PRIMARY KEY (`Spk_id`));").arg(databaseName).arg(tablename);
    if(!sql_query.exec(sql_str)){
        Logger::Error(QString("SQL - create Table '%1' error:").arg(tablename).arg(sql_query.lastError().text()));
        m_last_error = Error::CREATE_TABLE_ERROR;
    }else{
        Logger::Info(QString("SQL - Table '%1' created.").arg(tablename));
        m_last_error = Error::SUCCESS;
    }
}
