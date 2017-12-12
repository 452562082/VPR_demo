#include "mainwindow.h"
#include <QApplication>
#include "configparser.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConfigParser::ConfigSet();
    MainWindow w;
    w.show();

    return a.exec();
}
