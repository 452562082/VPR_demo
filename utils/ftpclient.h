#ifndef FTPCLIENT_H
#define FTPCLIENT_H

#include <QFile>
#include <QThread>
#include <QList>
#include <QMutex>
#include <initializer_list>
#include "utils/qftp.h"

namespace FtpTool {
    struct FtpOpInfo{
        enum class FtpOp{
            None,
            PUT,
            GET,
            REMOVE,
        };
        FtpOpInfo():m_ftp_op(FtpOp::None),m_params(){}
        FtpOpInfo(FtpOp ftp_op,const std::initializer_list<QString> &params){
            m_ftp_op = ftp_op;
            for(auto &item : params){
                m_params.append(item);
            }
        }
        FtpOpInfo(FtpOp ftp_op,const QStringList &params)
            :m_ftp_op(ftp_op),m_params(params){}
        FtpOpInfo(const FtpOpInfo& ftp_op_info){
            m_ftp_op = ftp_op_info.m_ftp_op;
            m_params = ftp_op_info.m_params;
        }
        FtpOpInfo& operator =(const FtpOpInfo& other){
            m_ftp_op = other.m_ftp_op;
            m_params = other.m_params;
            return *this;
        }
        FtpOp m_ftp_op;
        QStringList m_params;
    };

    class FtpOpInfoSafeQueue : public QObject
    {
        Q_OBJECT

    public:
        explicit FtpOpInfoSafeQueue(QObject* parent = 0):QObject(parent){}
        void addTail(const FtpOpInfo& ftp_op_info){
            m_mutex.lock();
            m_list.append(ftp_op_info);
            m_mutex.unlock();
        }

        FtpOpInfo getHead(){
            m_mutex.lock();
            FtpOpInfo ftpOpInfo;
            if(m_list.begin() != m_list.end()){
                ftpOpInfo = *m_list.begin();
                m_list.erase(m_list.begin());
            }
            m_mutex.unlock();
            return ftpOpInfo;
        }

        int size(){
            m_mutex.lock();
            int size = m_list.size();
            m_mutex.unlock();
            return size;
        }

    private:
        QList<FtpOpInfo> m_list;
        QMutex m_mutex;
    };

    class FtpThread: public QThread
    {
        Q_OBJECT

    public:
        static FtpThread* GetInstance();
        void stop();
        void setHost(const QString& host);
        void setPort(int port);
        void AddFtpOp(const FtpOpInfo& ftp_op_info);

    protected:
        void run();

    private:
        explicit FtpThread(QObject* parent = 0);
        ~FtpThread();
        void open();
        void close();
        void setTransferMode(QFtp::TransferMode mode);
        void connectToHost(const QString& host,int port);
        void login(const QString& username,const QString& password);
        void put(const QString& path,const QString& filename);
        void get(const QString& path,const QString& filename);
        void remove(const QString& path);

    private:
        static FtpThread* m_instance;
        QFtp m_ftp;
        QString m_host;
        int m_port;
        int m_idFtpOp;
        FtpOpInfoSafeQueue m_ftp_op_list;
        bool m_is_finished;
        bool m_is_runing;

    signals:
        void finished();

    private slots:
        void commandFinished(int,bool);
    };
}

#endif // FTPCLIENT_H
