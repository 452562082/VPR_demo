#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QFile>
#include <QDate>

class Logger
{
    enum class LogType{
        Info,
        Warning,
        Question,
        Error
    };

public:
    static void Info(const QString& str);
    static void Warning(const QString& str);
    static void Question(const QString& str);
    static void Error(const QString& str);

private:
    static Logger* m_instance;
    QFile m_file;
    QDate m_log_date;

private:
    Logger();
    ~Logger();
    void log(LogType type,const QString& str);
};

#endif // LOGGER_H
