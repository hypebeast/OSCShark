#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QListView>
#include <QPushButton>

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
};

#endif // MAINWINDOW_H
