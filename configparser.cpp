#include "configparser.h"

namespace {
    const char* configPath = "./config.ini";
}

ConfigParser* ConfigParser::m_instance = nullptr;
ConfigParser::ConfigParser() :
    m_db_hostName(""),
    m_db_port(0),
    m_rpc_ip(""),
    m_rpc_port(0),
    m_http_server_ip(""),
    m_http_server_port(0)
{
}

ConfigParser::~ConfigParser()
{
    if (ConfigParser::m_instance != nullptr) {
        delete ConfigParser::m_instance;
        ConfigParser::m_instance = nullptr;
    }
}

ConfigParser* ConfigParser::GetInstance()
{
    if(m_instance == nullptr){
        m_instance = new ConfigParser();
    }
    return m_instance;
}

void ConfigParser::Init()
{
    QSettings configParser(configPath,QSettings::IniFormat);
    //读取Database配置
    m_db_hostName = configParser.value("Database/Hostname").toString();
    m_db_port = configParser.value("Database/Port").toInt();
    //读取RPC配置
    m_rpc_ip = configParser.value("Rpc/Ip").toString();
    m_rpc_port = configParser.value("Rpc/Port").toInt();
    //读取http配置
    m_http_server_ip = configParser.value("Http/ServerIp").toString();
    m_http_server_port = configParser.value("Http/ServerPort").toInt();
}

QString ConfigParser::GetDB_HostName() const
{
    return m_db_hostName;
}

int ConfigParser::GetDB_Port() const
{
    return m_db_port;
}

QString ConfigParser::GetRPC_Ip() const
{
    return m_rpc_ip;
}

int ConfigParser::GetRPC_Port() const
{
    return m_rpc_port;
}

QString ConfigParser::GetHTTP_ServerIp() const
{
    return m_http_server_ip;
}

int ConfigParser::GetHTTP_ServerPort() const
{
    return m_http_server_port;
}
