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

#ifdef Q_OS_MACX
    if ( QSysInfo::MacintoshVersion > QSysInfo::MV_10_8 )
    {
        // fix Mac OS X 10.9 (mavericks) font issue
        // https://bugreports.qt-project.org/browse/QTBUG-32789
        QFont::insertSubstitution(".Lucida Grande UI", "Lucida Grande");
    }
#endif

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
    foreach (int port, activePorts) {
        createOscListener(port);
        printLogMessage("Listening to port: " + QString::number(port));
    }
}

void MainWindow::loadSettings()
{
    QSettings settings;
    availablePorts = settings.value("ports/available").value< QList<int> >();
    activePorts = settings.value("ports/active").value< QList<int> >();
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.setValue("ports/available", QVariant::fromValue< QList<int> >(availablePorts));
    settings.setValue("ports/active", QVariant::fromValue< QList<int> >(activePorts));
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
    leftMainLayout->addSpacing(10);

    QLabel *label1 = new QLabel;
    label1->setText("LISTENING TO OSC PORTS");
    leftMainLayout->addWidget(label1);

    lwListeningPorts = new QListWidget;
    leftMainLayout->addWidget(lwListeningPorts);

    foreach (int port, activePorts) {
        new QListWidgetItem(QString::number(port), lwListeningPorts);
    }

    QFont infoFont;
    infoFont.setPointSize(10);

    QLabel *infoText1 = new QLabel("Double click on item to stop listening");
    infoText1->setFont(infoFont);
    leftMainLayout->addWidget(infoText1);

    leftMainLayout->addSpacing(15);

    QLabel *label2 = new QLabel;
    label2->setText("AVAILABLE OSC PORTS");
    leftMainLayout->addWidget(label2);

    lwAvailablePorts = new QListWidget;
    leftMainLayout->addWidget(lwAvailablePorts);

    foreach (int port, availablePorts) {
        new QListWidgetItem(QString::number(port), lwAvailablePorts);
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

    leftMainLayout->addStretch();

    logOutput = new QTextEdit;
    logOutput->setMinimumWidth(650);
    logOutput->setMinimumHeight(400);
    logOutput->setReadOnly(true);
    rightMainLayout->addWidget(logOutput);

    QHBoxLayout *receivedOscLayout = new QHBoxLayout();
    rightMainLayout->addLayout(receivedOscLayout);

    QVBoxLayout *leftReceivedOscLayout = new QVBoxLayout();
    receivedOscLayout->addLayout(leftReceivedOscLayout);

    QVBoxLayout *rightReceivedOscLayout = new QVBoxLayout();
    receivedOscLayout->addLayout(rightReceivedOscLayout);

    QLabel *lReceivedAddresses = new QLabel("RECEIVED OSC ADDRESSES");
    leftReceivedOscLayout->addWidget(lReceivedAddresses);

    lwReceivedOscAddresses = new QListWidget();
    leftReceivedOscLayout->addWidget(lwReceivedOscAddresses);

    leftReceivedOscLayout->addWidget(new QLabel("Double click item to monitor this OSC address.\n"));

    QLabel *lListenToAddresses = new QLabel("ONLY LISTEN TO FOLLOWING OSC ADDRESSES");
    rightReceivedOscLayout->addWidget(lListenToAddresses);

    lwMonitoredOscAddresses = new QListWidget();
    rightReceivedOscLayout->addWidget(lwMonitoredOscAddresses);

    rightReceivedOscLayout->addWidget(new QLabel("Double click item to stop monitoring this OSC Address.\nWhen the list is empty every OSC address is monitored."));

    this->centralWidget()->setLayout(mainLayout);
    this->setFixedSize(900, 700);
}

void MainWindow::setupSignals()
{
    connect(bAddPort, SIGNAL(clicked()), this, SLOT(onAddPortClicked()));
    connect(lwAvailablePorts, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onAvailablePortClicked(QListWidgetItem*)));
    connect(lwListeningPorts, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onListeningPortClicked(QListWidgetItem*)));
    connect(lwReceivedOscAddresses, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onAddOscAddressClicked(QListWidgetItem*)));
    connect(lwMonitoredOscAddresses, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onRemoveMonitoredOscAddressClicked(QListWidgetItem*)));
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
            new QListWidgetItem(QString::number(port), lwAvailablePorts);

            saveSettings();
        }
    }
}

void MainWindow::onAvailablePortClicked(QListWidgetItem *item)
{
    int port = item->text().toInt();
    bool found = false;
    foreach (int p, activePorts) {
        if (p == port) {
            found = true;
            break;
        }
    }

    if (!found) {
        createOscListener(item->text().toInt());
        // Add the port to the listening ports list and save it
        activePorts.append(port);
        saveSettings();

        // Create a new list item
        new QListWidgetItem(item->text(), lwListeningPorts);

        printLogMessage("Listening to port: " + QString::number(port));
    } else {
        printLogMessage("Already listening to port: " + QString::number(port));
    }
}

void MainWindow::onListeningPortClicked(QListWidgetItem *item)
{
    int port = item->text().toInt();
    bool found = false;
    foreach (int p, activePorts) {
        if (p == port) {
            found = true;
            break;
        }
    }

    if (found) {
        removeOscListener(port);

        // Find the item in the list and remove it
        for (int i = 0; i < lwListeningPorts->count(); i++) {
            QListWidgetItem *li = lwListeningPorts->item(i);
            if (li->text() == item->text()) {
                li = lwListeningPorts->takeItem(i);
                delete li;
                break;
            }
        }
        printLogMessage("Stopped listening to port: " + QString::number(port));
    }
}

void MainWindow::onAddOscAddressClicked(QListWidgetItem *item)
{
    // Add the OSC address to the monitored OSC addresses, if it's not already
    bool found = false;
    foreach (QString address, monitoredOscAddresses) {
        if (address == item->text()) {
            found = true;
            break;
        }
    }

    if (!found) {
        monitoredOscAddresses.append(item->text());
        new QListWidgetItem(item->text(), lwMonitoredOscAddresses);

        printLogMessage("Added OSC Address \"" + item->text() + "\" for monitoring");
    }
}

void MainWindow::onRemoveMonitoredOscAddressClicked(QListWidgetItem *item)
{
    if (item == NULL) {
        return;
    }

    for (int i = 0; i < lwMonitoredOscAddresses->count(); i++) {
        QListWidgetItem *li = lwMonitoredOscAddresses->item(i);
        if (li->text() == item->text()) {
            monitoredOscAddresses.removeOne(item->text());
            lwMonitoredOscAddresses->takeItem(i);
//            delete lwMonitoredOscAddresses->takeItem(i);
            break;
        }
    }

    printLogMessage("Stopped monitoring OSC Address: " + item->text());
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
    connect(controller, SIGNAL(messageReceived(ReceivedOscMessage*)), this, SLOT(handleMessage(ReceivedOscMessage*)));

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

            activePorts.removeOne(port);

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

void MainWindow::processIncomingOscMessage(const ReceivedOscMessage *msg)
{
    // Check the OSC address and save it, if it's not already saved
    bool found = false;
    foreach (QString address, receivedOscAddresses) {
        if (address == msg->address) {
            found = true;
            break;
        }
    }

    if (!found) {
        receivedOscAddresses.append(msg->address);

        // Add it to the received OSC address list
        new QListWidgetItem(msg->address, lwReceivedOscAddresses);
    }

    if (filterOscMessage(msg->address) || monitoredOscAddresses.count() == 0) {
        logOscMessage(msg);
    }
}

bool MainWindow::filterOscMessage(const QString address)
{
    foreach (QString addr, monitoredOscAddresses) {
        if (addr == address) {
            return true;
        }
    }

    return false;
}

// Called when an OSC message was received
void MainWindow::handleMessage(ReceivedOscMessage *msg)
{
    processIncomingOscMessage(msg);
}

void MainWindow::logOscMessage(const ReceivedOscMessage *msg)
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
