#include <QLabel>
#include <QList>
#include <QHostAddress>
#include <QNetworkInterface>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "osclistener.h"

#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create the UI
    setupUi();

    // Try to get the local IP address
    lookupLocalAddresses();

    // FIXME just for testing!
//    OscListener *l = new OscListener("192.168.0.10", 8000);
//    l->start();
}

void MainWindow::setupUi()
{
    mainLayout = new QHBoxLayout;
    leftMainLayout = new QVBoxLayout;
    rightMainLayout = new QVBoxLayout;

    mainLayout->addLayout(leftMainLayout);
    mainLayout->addLayout(rightMainLayout);

    leIpAddress = new QLineEdit;
    leIpAddress->setReadOnly(true);
    leftMainLayout->addWidget(leIpAddress);

    QLabel *label1 = new QLabel;
    label1->setText("LISTENING TO OSC PORTS");
    leftMainLayout->addWidget(label1);

    lvListeningPorts = new QListView;
    lvListeningPorts->setMinimumHeight(50);
    lvListeningPorts->setMaximumHeight(150);
    leftMainLayout->addWidget(lvListeningPorts);

    QLabel *label2 = new QLabel;
    label2->setText("AVAILABLE OSC PORTS");
    leftMainLayout->addWidget(label2);

    lvAvailablePorts = new QListView;
    lvAvailablePorts->setMinimumHeight(50);
    lvAvailablePorts->setMaximumHeight(150);
    leftMainLayout->addWidget(lvAvailablePorts);

    leAddPort = new QLineEdit;
    leftMainLayout->addWidget(leAddPort);

    bAddPort = new QPushButton;
    bAddPort->setText("Add Port");
    leftMainLayout->addWidget(bAddPort);

    leftMainLayout->addSpacing(1);

    bClearView = new QPushButton;
    bClearView->setText("Clear Views");
    leftMainLayout->addWidget(bClearView);

    leftMainLayout->addStretch();

    logOutput = new QTextEdit;
    logOutput->setMinimumWidth(650);
    logOutput->setMinimumHeight(400);
    rightMainLayout->addWidget(logOutput);
    rightMainLayout->addStretch();

    this->centralWidget()->setLayout(mainLayout);

    this->setFixedSize(900, 700);
}

void MainWindow::lookupLocalAddresses()
{
    bool found = false;
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost)) {
             qDebug() << address.toString();

             QString ipaddress = "IP Address: " + address.toString();
             leIpAddress->setText(ipaddress);
             found = true;
             break;
        }
    }

    if (!found) {
        leIpAddress->setText("No IP Address found!");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
