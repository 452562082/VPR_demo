#include "logger.h"
#include <QDir>
#include <QDebug>

namespace {
    const char* logDirname = "./Log/";
}

Logger* Logger::m_instance = nullptr;
Logger::Logger()
{
    QDir log_dir;
    if(!log_dir.exists(logDirname)){
        log_dir.mkdir(logDirname);
    }

    QString logFilename = logDirname + QDate::currentDate().toString("Log_yyyy-MM-dd.txt");
    m_file.setFileName(logFilename);
    if(!m_file.open(QIODevice::WriteOnly | QIODevice::Append)){
        qDebug() << "LogError: can't open Log file.";
        return;
    }
}

Logger::~Logger()
{
    m_file.close();
    if (Logger::m_instance != nullptr) {
        delete Logger::m_instance;
        Logger::m_instance = nullptr;
    }
}

void Logger::Info(const QString& str)
{
    if(m_instance == nullptr){
        m_instance = new Logger();
    }
    m_instance->log(LogType::Info,str);
}

void Logger::Warning(const QString& str)
{
    if(m_instance == nullptr){
        m_instance = new Logger();
    }
    m_instance->log(LogType::Warning,str);
}

void Logger::Question(const QString& str)
{
    if(m_instance == nullptr){
        m_instance = new Logger();
    }
    m_instance->log(LogType::Question,str);
}

void Logger::Error(const QString& str)
{
    if(m_instance == nullptr){
        m_instance = new Logger();
    }
    m_instance->log(LogType::Error,str);
}

void Logger::log(LogType type,const QString& str)
{
    qDebug() << str;
    if(QDate::currentDate() != m_log_date){
        m_file.close();
        QString logFilename = logDirname + QDate::currentDate().toString("Log_yyyy-MM-dd.txt");
        m_file.setFileName(logFilename);
        if(!m_file.open(QIODevice::WriteOnly | QIODevice::Append)){
            qDebug() << "LogError: can't open Log file.";
            return;
        }
        m_log_date = QDate::currentDate();
    }

    QString logStr = "";
    QString nowDateTimeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    switch (type) {
    case LogType::Info:
        logStr += nowDateTimeStr + " Info: " + str + "\r\n";
        break;
    case LogType::Question:
        logStr += nowDateTimeStr + " Question: " + str + "\r\n";
        break;
    case LogType::Warning:
        logStr += nowDateTimeStr + " Warning: " + str + "\r\n";
        break;
    case LogType::Error:
        logStr += nowDateTimeStr + " Error: " + str + "\r\n";
        break;
    default:
        break;
    }

    m_file.write(logStr.toLocal8Bit());
    m_file.flush();
}
