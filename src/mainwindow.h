#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QList>
#include <QSettings>
#include <QCloseEvent>

#include "common.h"
#include "osclistenercontroller.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    QHBoxLayout *mainLayout;
    QVBoxLayout *leftMainLayout;
    QVBoxLayout *rightMainLayout;
    QLineEdit *leIpAddress;
    QListWidget *lwListeningPorts;
    QListWidget *lwAvailablePorts;
    QPushButton *bAddPort;
    QPushButton *bClearView;
    QTextEdit *logOutput;
    QListWidget *lwReceivedOscAddresses;
    QListWidget *lwMonitoredOscAddresses;
    QList<OscListenerController*> oscListeners;
    QList<int> availablePorts;
    QList<int> activePorts;
    QList<QString> receivedOscAddresses;
    QList<QString> monitoredOscAddresses;

    void setupUi();
    void loadSettings();
    void saveSettings();
    void setupSignals();
    void lookupLocalAddresses();
    void printLogMessage(const QString &);
    void logOscMessage(const ReceivedOscMessage*);
    void createOscListener(int);
    void removeOscListener(int);
    void processIncomingOscMessage(const ReceivedOscMessage*);
    bool filterOscMessage(const QString address);

private slots:
    void handleMessage(ReceivedOscMessage *);
    void onAddPortClicked();
    void onAvailablePortClicked(QListWidgetItem*);
    void onListeningPortClicked(QListWidgetItem*);
    void onClearViewsClicked();
    void onAddOscAddressClicked(QListWidgetItem*);
    void onRemoveMonitoredOscAddressClicked(QListWidgetItem*);
};


#endif // MAINWINDOW_H
