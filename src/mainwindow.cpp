#include <QLabel>
#include <QList>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QDateTime>
#include <QListWidgetItem>
#include <QFont>
#include <QInputDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "common.h"

#include <iostream>
using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Register QList<int> as a QT meta type
    qRegisterMetaType< QList<int> >("QList<int>");
    qRegisterMetaTypeStreamOperators< QList<int> >("QList<int>");

    QCoreApplication::setOrganizationName("SebastianRuml");
    QCoreApplication::setOrganizationDomain("sebastianruml.com");
    QCoreApplication::setApplicationName("OscMonitor");

    // Load the settings
    loadSettings();

    // Create the UI
    setupUi();

    // Connect to all signals
    setupSignals();

    // Try to get the local IP address
    lookupLocalAddresses();

    // Start listening on all saved listening ports
    foreach (int port, listeningPorts) {
        createOscListener(port);
        printLogMessage("Listening to port: " + QString::number(port));
    }
}

void MainWindow::loadSettings()
{
    QSettings settings;
    availablePorts = settings.value("ports/available").value< QList<int> >();
    listeningPorts = settings.value("ports/active").value< QList<int> >();
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.setValue("ports/available", QVariant::fromValue< QList<int> >(availablePorts));
    settings.setValue("ports/active", QVariant::fromValue< QList<int> >(listeningPorts));
}

void MainWindow::setupUi()
{
    mainLayout = new QHBoxLayout;
    leftMainLayout = new QVBoxLayout;
    leftMainLayout->setContentsMargins(0, 0, 0, 0);
    rightMainLayout = new QVBoxLayout;
    rightMainLayout->setContentsMargins(0, 0, 0, 0);

    mainLayout->addLayout(leftMainLayout);
    mainLayout->addLayout(rightMainLayout);

    leIpAddress = new QLineEdit;
    leIpAddress->setReadOnly(true);
    leftMainLayout->addWidget(leIpAddress);

    leftMainLayout->addSpacing(5);

    QLabel *label1 = new QLabel;
    label1->setText("LISTENING TO OSC PORTS");
    leftMainLayout->addWidget(label1);

    lvListeningPorts = new QListWidget;
    lvListeningPorts->setMinimumHeight(50);
    lvListeningPorts->setMaximumHeight(150);
    leftMainLayout->addWidget(lvListeningPorts);

    foreach (int port, listeningPorts) {
        new QListWidgetItem(QString::number(port), lvListeningPorts);
    }

    QFont infoFont;
    infoFont.setPointSize(10);

    QLabel *infoText1 = new QLabel("Double click on item to stop listening");
    infoText1->setFont(infoFont);
    infoText1->setMargin(0);
    leftMainLayout->addWidget(infoText1);

    leftMainLayout->addSpacing(15);

    QLabel *label2 = new QLabel;
    label2->setText("AVAILABLE OSC PORTS");
    leftMainLayout->addWidget(label2);

    lvAvailablePorts = new QListWidget;
    lvAvailablePorts->setMinimumHeight(50);
    lvAvailablePorts->setMaximumHeight(150);
    leftMainLayout->addWidget(lvAvailablePorts);

    foreach (int port, availablePorts) {
        new QListWidgetItem(QString::number(port), lvAvailablePorts);
    }

    QLabel *infoText2 = new QLabel("Double click on item to start listening");
    infoText2->setFont(infoFont);
    leftMainLayout->addWidget(infoText2);

    bAddPort = new QPushButton;
    bAddPort->setText("Add Port");
    leftMainLayout->addWidget(bAddPort);

    leftMainLayout->addSpacing(30);

    bClearView = new QPushButton;
    bClearView->setText("Clear Views");
    leftMainLayout->addWidget(bClearView);

    logOutput = new QTextEdit;
    logOutput->setMinimumWidth(650);
    logOutput->setMinimumHeight(400);
    logOutput->setReadOnly(true);
    rightMainLayout->addWidget(logOutput);
    rightMainLayout->addStretch();

    this->centralWidget()->setLayout(mainLayout);
    this->setFixedSize(900, 700);
}

void MainWindow::setupSignals()
{
    connect(bAddPort, SIGNAL(clicked()), this, SLOT(onAddPortClicked()));
    connect(lvAvailablePorts, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onAvailablePortClicked(QListWidgetItem*)));
    connect(lvListeningPorts, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onListeningPortClicked(QListWidgetItem*)));
    connect(bClearView, SIGNAL(clicked()), this, SLOT(onClearViewsClicked()));
}

void MainWindow::onAddPortClicked()
{
    bool ok;
    int port = QInputDialog::getInt(this, tr("Add Port"), tr("Port:"),
                                         0, 0, 65535, 1, &ok);

    if (ok) {
        bool exists = false;
        foreach (int p, availablePorts) {
            if (p == port) {
                exists = true;
                break;
            }
        }

        if (!exists) {
            availablePorts.append(port);
            new QListWidgetItem(QString::number(port), lvAvailablePorts);

            saveSettings();
        }
    }
}

void MainWindow::onAvailablePortClicked(QListWidgetItem *item)
{
    int port = item->text().toInt();
    bool found = false;
    foreach (int p, listeningPorts) {
        if (p == port) {
            found = true;
            break;
        }
    }

    if (!found) {
        createOscListener(item->text().toInt());
        // Add the port to the listening ports list and save it
        listeningPorts.append(port);
        saveSettings();

        // Create a new list item
        new QListWidgetItem(item->text(), lvListeningPorts);

        printLogMessage("Listening to port: " + QString::number(port));
    } else {
        printLogMessage("Already listening to port: " + QString::number(port));
    }
}

void MainWindow::onListeningPortClicked(QListWidgetItem *item)
{
    int port = item->text().toInt();
    bool found = false;
    foreach (int p, listeningPorts) {
        if (p == port) {
            found = true;
            break;
        }
    }

    if (found) {
        removeOscListener(port);

        // Find the item in the list and remove it
        for (int i = 0; i < lvListeningPorts->count(); i++) {
            QListWidgetItem *li = lvListeningPorts->item(i);
            if (li->text() == item->text()) {
                li = lvListeningPorts->takeItem(i);
                delete li;
                break;
            }
        }
        printLogMessage("Stopped listening to port: " + QString::number(port));
    }
}

void MainWindow::createOscListener(int port)
{
    // Check if an OSC listener for the given port is already created
    for (int i = 0; i < oscListeners.size(); i++) {
        if (oscListeners.at(i)->Port() == port) {
            return;
        }
    }

    // Create an new OSC listener
    OscListenerController *controller = new OscListenerController(port);

    // Connect to the message received signal
    connect(controller, SIGNAL(messageReceived(OscMessageContainer*)), this, SLOT(handleMessage(OscMessageContainer*)));

    // Start listening
    controller->Start();

    oscListeners.append(controller);
}

void MainWindow::removeOscListener(int port)
{
    // Try to find the right OSC listener for the given port
    for (int i = 0; i < oscListeners.size(); i++) {
        if (oscListeners.at(i)->Port() == port) {
            OscListenerController *controller = oscListeners.at(i);
            controller->Stop();
            oscListeners.removeAt(i);
            delete controller;

            listeningPorts.removeOne(port);

            saveSettings();

            break;
        }
    }
}

void MainWindow::lookupLocalAddresses()
{
    bool found = false;
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost)) {
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

void MainWindow::handleMessage(OscMessageContainer *msg)
{
    logOscMessage(msg);
}

void MainWindow::logOscMessage(const OscMessageContainer *msg)
{
    QString logStr;
    logStr.append("[");
    logStr.append(QString::number(msg->port));
    logStr.append("] ");
    logStr.append(msg->address + " " + msg->typeTags);
    foreach (QString arg, msg->arguments) {
        logStr.append(" ");
        logStr.append(arg);
    }

    printLogMessage(logStr);
}

void MainWindow::printLogMessage(const QString &msg)
{
    QDateTime time = QDateTime::currentDateTime();
    QString output = time.toString("dd.MM.yyyy hh:mm:ss");
    output.append(" - ");
    output.append(msg);

    logOutput->append(output);
}

void MainWindow::onClearViewsClicked()
{
    logOutput->clear();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // Stop every OSC listener
    for (int i = 0; i < oscListeners.count(); i++) {
        OscListenerController *listener = oscListeners.at(i);
        listener->Stop();
    }

    event->accept();
}

MainWindow::~MainWindow()
{
    delete ui;
}
