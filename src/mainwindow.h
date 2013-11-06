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
#include <QCheckBox>

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
    QPushButton *bExport;
    QTextEdit *logOutput;
    QListWidget *lwReceivedOscAddresses;
    QListWidget *lwMonitoredOscAddresses;
    QList<OscListenerController*> oscListeners;
    QList<int> availablePorts;
    QList<int> activePorts;
    QList<QString> receivedOscAddresses;
    QList<QString> monitoredOscAddresses;
    QList<OscMessageContainer> receivedMessages;
    QCheckBox *cbShowTimestamps;
    QCheckBox *cbShowOnlyUpdatedMessages;
    bool showTimestamps;
    bool showOnlyUpdatedMessages;

    void setupUi();
    void loadSettings();
    void saveSettings();
    void setupSignals();
    void lookupLocalAddresses();
    void printLogMessage(const QString &);
    void logOscMessage(const OscMessageContainer*);
    void createOscListener(int);
    void removeOscListener(int);
    void processIncomingOscMessage(const OscMessageContainer*);
    bool filterOscMessage(const QString address);

private slots:
    void handleMessage(OscMessageContainer *);
    void onAddPortClicked();
    void onAvailablePortClicked(QListWidgetItem*);
    void onListeningPortClicked(QListWidgetItem*);
    void onClearViewsClicked();
    void onAddOscAddressClicked(QListWidgetItem*);
    void onRemoveMonitoredOscAddressClicked(QListWidgetItem*);
    void onExportClicked();
    void onShowTimestampsChecked(int state);
    void onShowOnlyUpdatedOscMessages(int state);
};


#endif // MAINWINDOW_H
