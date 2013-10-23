#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QByteArray>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile qss(":/stylesheets/stylesheet.qss");
    qss.open(QFile::ReadOnly);
    a.setStyleSheet(qss.readAll());
    qss.close();
    MainWindow w;
    w.show();

    return a.exec();
}
