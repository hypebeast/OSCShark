#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QListView>
#include <QPushButton>
#include <QList>

#include "common.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QHBoxLayout *mainLayout;
    QVBoxLayout *leftMainLayout;
    QVBoxLayout *rightMainLayout;
    QLineEdit *leIpAddress;
    QListView *lvListeningPorts;
    QListView *lvAvailablePorts;
    QLineEdit *leAddPort;
    QPushButton *bAddPort;
    QPushButton *bClearView;
    QTextEdit *logOutput;

    void setupUi();
    void lookupLocalAddresses();
    void printLogMessage(const QString &);
    void logOscMessage(const OscMessageContainer*);

private slots:
    void handleMessage(OscMessageContainer *msg);
};


#endif // MAINWINDOW_H
