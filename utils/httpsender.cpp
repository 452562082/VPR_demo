#include "httpsender.h"
#include <QUuid>
#include <QFile>
#include <QUrl>
#include <QHttpPart>
#include "utils/logger.h"
#include "configparser.h"

HttpSender* HttpSender::m_instance = nullptr;
HttpSender::HttpSender(QObject *parent) : QObject(parent)
{
    connect(&m_manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(finished(QNetworkReply*)));
}

HttpSender::~HttpSender()
{
    if(HttpSender::m_instance != nullptr){
        delete HttpSender::m_instance;
        HttpSender::m_instance = nullptr;
    }
}

HttpSender* HttpSender::GetInstance()
{
    if(m_instance == nullptr){
        m_instance = new HttpSender();
    }
    return m_instance;
}

//void HttpSender::setHost(const QString& host)
//{
//    m_host = host;
//}

//void HttpSender::setPort(int port)
//{
//    m_port = port;
//}

void HttpSender::uploadText(const QByteArray& text)
{
    QString url = "http://" + ConfigParser::GetInstance()->GetHTTP_ServerIp() + ":" + QString::number(ConfigParser::GetInstance()->GetHTTP_ServerPort(),10) + "/uploadText";

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QString uuidboundry = QUuid::createUuid().toString();
    QNetworkRequest request = QNetworkRequest(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader,"multipart/form-data;boundary="+uuidboundry);
    QHttpPart textPart;
    textPart.setHeader(QNetworkRequest::ContentDispositionHeader,QString("form-data; name=\"%1\"").arg("text"));
    textPart.setBody(text);
    multiPart->append(textPart);
    multiPart->setBoundary(uuidboundry.toLatin1());
    QNetworkReply *reply = m_manager.post(request,multiPart);
    multiPart->setParent(reply);//通过reply释放multiPart和file
}

void HttpSender::uploadFile(const QString& filePath,const QString& path)
{
    QString url = "http://" + ConfigParser::GetInstance()->GetHTTP_ServerIp() + ":" + QString::number(ConfigParser::GetInstance()->GetHTTP_ServerPort(),10) + "/uploadFile";
    QFile *file = new QFile(filePath);
    if(!file->open(QIODevice::ReadOnly)){
        Logger::Error("HTTP - upload file can't open.");
        return;
    }

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QString uuidboundry = QUuid::createUuid().toString();
    QNetworkRequest request = QNetworkRequest(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader,"multipart/form-data;boundary="+uuidboundry);
    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader,QString("form-data; name=\"%1\"; filename=\"%2\"").arg("file").arg(path));
    QString suffix = filePath.right(filePath.length() - filePath.lastIndexOf('.'));
    filePart.setHeader(QNetworkRequest::ContentTypeHeader,suffix2contentType(suffix));
    filePart.setBodyDevice(file);
    multiPart->append(filePart);
    multiPart->setBoundary(uuidboundry.toLatin1());
    QNetworkReply *reply = m_manager.post(request,multiPart);
    multiPart->setParent(reply);//通过reply释放multiPart和file
}

void HttpSender::downloadFile(const QString& path,const QString& filePath)
{
    QString url = "http://" + ConfigParser::GetInstance()->GetHTTP_ServerIp() + ":" + QString::number(ConfigParser::GetInstance()->GetHTTP_ServerPort(),10) + "/downloadFile?filename=" + path;
    
    QNetworkRequest request = QNetworkRequest(QUrl(url));
    QNetworkReply *reply = m_manager.get(request);
    m_filenameMap.insert(reply,filePath);
    connect(reply,SIGNAL(readyRead()),this,SLOT(on_readyRead()));
}

void HttpSender::removeFile(const QString& path)
{
    QString url = "http://" + ConfigParser::GetInstance()->GetHTTP_ServerIp() + ":" + QString::number(ConfigParser::GetInstance()->GetHTTP_ServerPort(),10) + "/removeFile?filename=" + path;
    QNetworkRequest request = QNetworkRequest(QUrl(url));
    m_manager.get(request);
}

void HttpSender::on_readyRead()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    QFile file(m_filenameMap.value(reply));
    if(!file.open(QIODevice::WriteOnly | QIODevice::Append)){
        Logger::Error("HTTP - download file can't open.");
        return;
    }

    file.write(reply->readAll());
    file.close();
}

void HttpSender::finished(QNetworkReply* reply)
{
    QVariant statuscode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(statuscode == 200 && reply->error() == QNetworkReply::NoError)
    {
        Logger::Info("HTTP - operation successfully.");
    }else{
        Logger::Error(QString("HTTP - operation failure - %1").arg(reply->errorString()));
    }
    if(m_filenameMap.contains(reply)){
        m_filenameMap.remove(reply);
    }
    reply->deleteLater();
    emit finished();
}

QString HttpSender::suffix2contentType(const QString &suffix)
{
    QString contentType = "";
    if(suffix.compare(".jpg") == 0){
        contentType = "image/jpeg";
    }else if(suffix.compare(".png") == 0){
        contentType = "image/png";
    }else if(suffix.compare(".pic") == 0){
        contentType = "application/x-pic";
    }
    return contentType;
}
