#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include <QSettings>

class ConfigParser
{
public:
    static ConfigParser* GetInstance();
    void Init();
    QString GetDB_HostName() const;
    int GetDB_Port() const;
    QString GetRPC_Ip() const;
    int GetRPC_Port() const;
    QString GetHTTP_ServerIp() const;
    int GetHTTP_ServerPort() const;

private:
    ConfigParser();
    ~ConfigParser();

private:
    static ConfigParser* m_instance;
    QString m_db_hostName;
    int m_db_port;
    QString m_rpc_ip;
    int m_rpc_port;
    QString m_http_server_ip;
    int m_http_server_port;
};

#endif // CONFIGPARSER_H
