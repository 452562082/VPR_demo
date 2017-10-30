#include "ftpclient.h"
#include <QDebug>
#include <QApplication>

namespace {
    const char* username = "asv_ftp";
    const char* password = "kst_ftp123";
}

namespace FtpTool {
    FtpThread* FtpThread::m_instance = nullptr;
    FtpThread::FtpThread(QObject* parent)
        :QThread(parent),m_is_finished(false),m_is_runing(true)
    {
    }

    FtpThread::~FtpThread()
    {
        close();
        if (FtpThread::m_instance != nullptr) {
            delete FtpThread::m_instance;
            FtpThread::m_instance = nullptr;
        }
    }

    FtpThread* FtpThread::GetInstance()
    {
        if(m_instance == nullptr){
            m_instance = new FtpThread();
        }
        return m_instance;
    }

    void FtpThread::setHost(const QString &host)
    {
        m_host = host;
    }

    void FtpThread::setPort(int port)
    {
        m_port = port;
    }

    void FtpThread::AddFtpOp(const FtpOpInfo& ftp_op_info)
    {

        m_ftp_op_list.addTail(ftp_op_info);
    }

    void FtpThread::open()
    {
        connectToHost(m_host,m_port);
        login(username,password);
        setTransferMode(QFtp::Active);
    }

    void FtpThread::close()
    {
        m_ftp.close();
    }

    void FtpThread::run()
    {
        connect(&m_ftp,SIGNAL(commandFinished(int,bool)),this,SLOT(commandFinished(int,bool)));
        open();
        while(m_is_runing){
//            if(m_ftp.state() != QFtp::LoggedIn){
//                qDebug() << m_ftp.state();
//                open();
//            }

            FtpTool::FtpOpInfo ftp_op_info = m_ftp_op_list.getHead();
            switch (ftp_op_info.m_ftp_op) {
            case FtpOpInfo::FtpOp::None:
//                emit finished();
                break;
            case FtpOpInfo::FtpOp::PUT:
                if(ftp_op_info.m_params.size() >= 2){
                    put(ftp_op_info.m_params[0],ftp_op_info.m_params[1]);
                }
                break;
            case FtpOpInfo::FtpOp::GET:
                if(ftp_op_info.m_params.size() >= 2){
                    get(ftp_op_info.m_params[0],ftp_op_info.m_params[1]);
                }
                break;
            case FtpOpInfo::FtpOp::REMOVE:
                if(ftp_op_info.m_params.size() >= 1){
                    remove(ftp_op_info.m_params[0]);
                }
                break;
            default:
                break;
            }
            QThread::msleep(100);
        }
    }

    void FtpThread::stop()
    {
        m_is_runing = false;
    }

    void FtpThread::setTransferMode(QFtp::TransferMode mode)
    {
        m_idFtpOp = m_ftp.setTransferMode(mode);
        while(!m_is_finished){
            qApp->processEvents();
        }
    }

    void FtpThread::connectToHost(const QString& host,int port)
    {
        m_idFtpOp = m_ftp.connectToHost(host,port);
        while(!m_is_finished){
            qApp->processEvents();
        }
    }

    void FtpThread::login(const QString& username,const QString& password)
    {
        m_idFtpOp = m_ftp.login(username,password);
        while(!m_is_finished){
            qApp->processEvents();
        }
    }

    void FtpThread::put(const QString& path,const QString &filename)
    {
        m_is_finished = false;
        QFile file(filename);
        if(!file.open(QIODevice::ReadOnly)){
            qDebug() << "FtpError: can't open file in put operation.";
            return;
        }
        QByteArray filedata = file.readAll();
        file.close();

        m_idFtpOp = m_ftp.put(filedata,path);
        while(!m_is_finished){
            qApp->processEvents();
        }
    }

    void FtpThread::get(const QString& path,const QString& filename)
    {
        QFile *file = new QFile;
        file->setFileName(filename);
        if(!file->open(QIODevice::WriteOnly)){
            qDebug() << "FtpError: can't open file in get operation.";
            return;
        }

        m_idFtpOp = m_ftp.get(path,file);
        while(!m_is_finished){
            qApp->processEvents();
        }
    }

    void FtpThread::remove(const QString& path)
    {
        m_idFtpOp = m_ftp.remove(path);
        while(!m_is_finished){
            qApp->processEvents();
        }
    }

    void FtpThread::commandFinished(int id,bool is_error)
    {
        QFtp *ftp = qobject_cast<QFtp*>(sender());
        QString commandStr = "";
        switch(ftp->currentCommand()){
        case QFtp::None:
            commandStr = "None";
            break;
        case QFtp::SetTransferMode:
            commandStr = "SetTransferMode";
            break;
        case QFtp::SetProxy:
            commandStr = "SetProxy";
            break;
        case QFtp::ConnectToHost:
            commandStr = "ConnectToHost";
            break;
        case QFtp::Login:
            commandStr = "Login";
            break;
        case QFtp::Close:
            commandStr = "Close";
            break;
        case QFtp::List:
            commandStr = "List";
            break;
        case QFtp::Cd:
            commandStr = "Cd";
            break;
        case QFtp::Get:
            commandStr = "Get";
            break;
        case QFtp::Put:
            commandStr = "Put";
            break;
        case QFtp::Remove:
            commandStr = "Remove";
            break;
        case QFtp::Mkdir:
            commandStr = "Mkdir";
            break;
        case QFtp::Rmdir:
            commandStr = "Rmdir";
            break;
        case QFtp::Rename:
            commandStr = "Rename";
            break;
        case QFtp::RawCommand:
            commandStr = "RawCommand";
            break;
        default:
            break;
        }

        if(is_error){
            qDebug() << "FtpError: " << commandStr << "failed, " << m_ftp.errorString();
        }else{
            if(ftp->currentCommand() == QFtp::Get){
                QFile *file = qobject_cast<QFile*>(m_ftp.currentDevice());
                if(file){
                    file->close();
                    file->deleteLater();
                }
            }
            qDebug() << "FtpInfo: " << commandStr << "success.";
        }
        if(m_idFtpOp == id){
            m_is_finished = true;
        }
    }
}
