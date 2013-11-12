/*
 * The MIT License (MIT)
 *
 * Copyright (c) <2013> <Sebastian Ruml>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

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
    QPushButton *bDeletePort;
    QPushButton *bClearView;
    QPushButton *bExport;
    QTextEdit *logView;
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
    bool showOnlyUpdatedAddresses;
    QList<QString> loggedOscAddresses;

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
    void updateLogView();

private slots:
    void handleMessage(OscMessageContainer *);
    void onAddPortClicked();
    void onDeletePortClicked();
    void onAvailablePortClicked(QListWidgetItem*);
    void onListeningPortClicked(QListWidgetItem*);
    void onClearViewsClicked();
    void onAddOscAddressClicked(QListWidgetItem*);
    void onRemoveMonitoredOscAddressClicked(QListWidgetItem*);
    void onExportClicked();
    void onShowTimestampsChecked(int state);
    void onShowOnlyUpdatedOscAddresses(int state);
};


#endif // MAINWINDOW_H
