#include "mainwindow.h"
#include <QApplication>
#include "configparser.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConfigParser::ConfigSet();
    MainWindow w;
    w.show();

//    HttpSender *http_sender = HttpSender::GetInstance();
//    http_sender->setHost("127.0.0.1");
//    http_sender->setPort(8080);
//    QString filename = "C:/Users/Yu/Desktop/Head/1.png";
//    QString ftp_filename = "/1212121" + filename.right(filename.length() - filename.lastIndexOf('.'));
////    http_sender->uploadFile(filename,ftp_filename);
////    http_sender->downloadFile("1212121.png","C:/Users/Yu/Desktop/1.png");
//    http_sender->removeFile("1212121.png");

    return a.exec();
}
