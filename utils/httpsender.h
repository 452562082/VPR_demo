#ifndef HTTPSENDER_H
#define HTTPSENDER_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QMap>

class HttpSender : public QObject
{
    Q_OBJECT
public:
    static HttpSender* GetInstance();
    void setHost(const QString& host);
    void setPort(int port);
    /// @brief 上传文本(未开通)
    /// @param text 文本
    void uploadText(const QByteArray& text);
    /// @brief 上传文件
    /// @param filePath 文件路径
    /// @param path 服务端文件名
    void uploadFile(const QString& filePath,const QString& path);
    /// @brief 下载文件(不支持断点续传)
    /// @param path 服务端文件名
    /// @param filePath 文件保存路径
    void downloadFile(const QString& path,const QString& filePath);
    /// @brief 删除文件
    /// @param path 服务端文件名
    void removeFile(const QString& path);

private:
    explicit HttpSender(QObject *parent = nullptr);
    ~HttpSender();
    QString suffix2contentType(const QString &suffix);

signals:
    void finished();

private slots:
    void on_readyRead();
    void finished(QNetworkReply*);

private:
    static HttpSender* m_instance;
    QNetworkAccessManager m_manager;
    QString m_host;
    int m_port;
    QMap<QNetworkReply*,QString> m_filenameMap;
//    QString m_filePath;
};

#endif // HTTPSENDER_H
