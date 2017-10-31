#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include <QSettings>
#include "utils/rpc_kvp_tool.h"
#include "soundsdata_db.h"
#include "utils/httpsender.h"

class ConfigParser
{
public:
    static void ConfigSet(){
        QSettings configParser("config.ini",QSettings::IniFormat);
        //读取Database配置
        SoundsData_db *db = SoundsData_db::GetInstance();
        QString db_hostName = configParser.value("Database/Hostname").toString();
        db->setHostName(db_hostName);
        int db_port = configParser.value("Database/Port").toInt();
        db->setPort(db_port);
        //读取Kvp配置
        RPC_Kvp_Tool *kvp_tool = RPC_Kvp_Tool::GetInstance();
        QString kvp_ip = configParser.value("Kvp/Ip").toString();
        kvp_tool->setKvpIp(kvp_ip.toStdString());
        int kvp_port = configParser.value("Kvp/Port").toInt();
        kvp_tool->setKvpPort(kvp_port);
        //读取http配置
        HttpSender *http_sender = HttpSender::GetInstance();
        QString http_server_ip = configParser.value("Http/ServerIp").toString();
        http_sender->setHost(http_server_ip);
        int http_server_port = configParser.value("Http/ServerPort").toInt();
        http_sender->setPort(http_server_port);
    }
};

#endif // CONFIGPARSER_H
