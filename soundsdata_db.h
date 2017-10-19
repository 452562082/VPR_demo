#ifndef SoundsData_db_H
#define SoundsData_db_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDate>

struct RegistrantInfo{
    RegistrantInfo():spk_id(""),name(""),head_path(""),registration_time(QDateTime::currentDateTime()){}
    RegistrantInfo(const RegistrantInfo& info):
        spk_id(info.spk_id),
        name(info.name),
        head_path(info.head_path),
        registration_time(info.registration_time){}
    QString spk_id;
    QString name;
    QString head_path;
    QDateTime registration_time;
};

class SoundsData_db
{
    enum class Error{
        SUCCESS,
        DB_OPEN_ERROR,
        CREATE_SCHEMA_ERROR,
        CREATE_TABLE_ERROR,
//        IDCARD_IS_EXISTED,
        INSERT_INTO_TABLE_ERROR,
        SELECT_FROM_TABLE_ERROR,
        DELETE_FROM_TABLE_ERROR,
    };

public:
    static SoundsData_db* GetInstance();
//    bool IsRegistrantExist(const QString &id_card);
    bool AddRegistrantInfo(const QString &spk_id, const QString &name, const QString &head_path);
    bool GetAllRegistrantsInfo(QVector<RegistrantInfo> &ret);
    bool GetRegistrantInfoBySpkId(const QString &spk_id, RegistrantInfo &ret);
    bool DeleteRegistrantInfoBySpkId(const QString &spk_id);
    Error GetLastError() const;

private:
    SoundsData_db();
    ~SoundsData_db();
    bool open();
    void close();
    void createDatabase();
    void createRegistrantsInfoTable();

private:
    static SoundsData_db* m_instance;
    QSqlDatabase m_database;
    Error m_last_error;
};

#endif // SoundsData_db_H
