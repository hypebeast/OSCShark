#include <QLabel>
#include <QList>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QDateTime>
#include <QListWidgetItem>
#include <QFont>
#include <QInputDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QIcon>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "common.h"
#include "exportdialog.h"

#include <iostream>
using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

// Font Fix for Mac OS X Mavericks
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
    QCoreApplication::setApplicationName("OscShark");

    // Set the icon
    //this->setWindowIcon(QIcon(":/icons/icon.png"));

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
    showTimestamps = settings.value("main/showtimestamps").value<bool>();
    showOnlyUpdatedAddresses = settings.value("main/showonlyupdatedmessages").value<bool>();
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.setValue("ports/available", QVariant::fromValue< QList<int> >(availablePorts));
    settings.setValue("ports/active", QVariant::fromValue< QList<int> >(activePorts));
    settings.setValue("main/showtimestamps", showTimestamps);
    settings.setValue("main/showonlyupdatedmessages", showOnlyUpdatedAddresses);
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

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    leftMainLayout->addLayout(buttonLayout);

    bAddPort = new QPushButton;
    bAddPort->setText("Add Port");
    buttonLayout->addWidget(bAddPort);
    //leftMainLayout->addWidget(bAddPort);

    bDeletePort = new QPushButton;
    bDeletePort->setText("Delete");
    buttonLayout->addWidget(bDeletePort);

    leftMainLayout->addSpacing(30);

    cbShowTimestamps = new QCheckBox("Show Timestamps");
    cbShowTimestamps->setChecked(showTimestamps);
    leftMainLayout->addWidget(cbShowTimestamps);

    cbShowOnlyUpdatedMessages = new QCheckBox("Show only updated Messages");
    cbShowOnlyUpdatedMessages->setChecked(showOnlyUpdatedAddresses);
    leftMainLayout->addWidget(cbShowOnlyUpdatedMessages);
    leftMainLayout->addSpacing(10);

    bClearView = new QPushButton;
    bClearView->setText("Clear Messages");
    leftMainLayout->addWidget(bClearView);

    bExport = new QPushButton;
    bExport->setText("Export");
    leftMainLayout->addWidget(bExport);

    leftMainLayout->addSpacing(17);

    QLabel *lVersion = new QLabel("OSC Shark v" +
                            QString("%1.%2.%3").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_BUILD));
    leftMainLayout->addWidget(lVersion);

    logView = new QTextEdit;
    logView->setMinimumWidth(650);
    logView->setMinimumHeight(400);
    logView->setReadOnly(true);
    rightMainLayout->addWidget(logView);

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
    connect(bDeletePort, SIGNAL(clicked()), this, SLOT(onDeletePortClicked()));
    connect(lwAvailablePorts, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onAvailablePortClicked(QListWidgetItem*)));
    connect(lwListeningPorts, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onListeningPortClicked(QListWidgetItem*)));
    connect(lwReceivedOscAddresses, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onAddOscAddressClicked(QListWidgetItem*)));
    connect(lwMonitoredOscAddresses, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onRemoveMonitoredOscAddressClicked(QListWidgetItem*)));
    connect(bClearView, SIGNAL(clicked()), this, SLOT(onClearViewsClicked()));
    connect(bExport, SIGNAL(clicked()), this, SLOT(onExportClicked()));
    connect(cbShowTimestamps, SIGNAL(stateChanged(int)), this, SLOT(onShowTimestampsChecked(int)));
    connect(cbShowOnlyUpdatedMessages, SIGNAL(stateChanged(int)), this, SLOT(onShowOnlyUpdatedOscAddresses(int)));
}

void MainWindow::onAddPortClicked()
{
    bool ok, error = false;
//    int port = QInputDialog::getInt(this, tr("Add Port"), tr("Port:"),
//                                         0, 0, 65535, 1, &ok);
    QString strPort = QInputDialog::getText(this, tr("Add Port"), tr("Port"), QLineEdit::Normal,
                          tr(""), &ok);
    if (ok) {
        int port = strPort.toInt(&ok, 10);
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
        else
            error = true;
    }

    if (error) {
        QMessageBox box(this);
        box.setText("Could not add port");
        box.exec();
    }
}

void MainWindow::onDeletePortClicked()
{
    // Check if a port is selected
    QList<QListWidgetItem*> items = lwAvailablePorts->selectedItems();
    if (items.count() != 1) {
        return;
    }

    // Remove port and item
    int port = items[0]->text().toInt();
    availablePorts.removeOne(port);
    delete lwAvailablePorts->takeItem(lwAvailablePorts->row(items[0]));
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

void MainWindow::processIncomingOscMessage(const OscMessageContainer *msg)
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
        receivedMessages.append(*msg);
        if (showOnlyUpdatedAddresses) {
            if (!loggedOscAddresses.contains(msg->address)) {
                loggedOscAddresses.append(msg->address);
                logOscMessage(msg);
            }
        }
        else
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
void MainWindow::handleMessage(OscMessageContainer *msg)
{
    processIncomingOscMessage(msg);
}

void MainWindow::logOscMessage(const OscMessageContainer *msg)
{
    QString logStr;
    if (showTimestamps) {
        logStr.append(msg->time.toString("dd.MM.yyyy hh:mm:ss"));
        logStr.append(" - ");
    }
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
    QString output;
    output.append(msg);

    logView->append(output);
}

void MainWindow::updateLogView()
{
    logView->clear();
    loggedOscAddresses.clear();
    foreach (OscMessageContainer msg, receivedMessages) {
        if (showOnlyUpdatedAddresses) {
            if (!loggedOscAddresses.contains(msg.address)) {
                loggedOscAddresses.append(msg.address);
                logOscMessage(&msg);
            }
        }
        else
            logOscMessage(&msg);
    }
}

void MainWindow::onClearViewsClicked()
{
    logView->clear();
    receivedMessages.clear();
}

void MainWindow::onExportClicked()
{
    // Don't export if there are no messages
    if (receivedMessages.count() <= 0) {
        QMessageBox msgBox(this);
        msgBox.setText("There are no OSC Messages to export.");
        msgBox.exec();
        return;
    }

    // Show the export dialog
    ExportDialog dlg(this);
    int result = dlg.exec();

    if (result == QDialog::Accepted) {
        QString fileName = dlg.fileNameExport;
        if (!fileName.isEmpty() && !fileName.isNull()) {
            QFile file(fileName);
            if (file.open(QFile::WriteOnly | QFile::Truncate)) {
                QTextStream stream(&file);
                foreach (OscMessageContainer msg, receivedMessages) {
                    if (dlg.exportTimestamp) {
                        stream << msg.time.toString("dd.MM.yyyy hh:mm:ss") << ",";
                    }
                    stream << QString::number(msg.port) << "," << msg.address << msg.typeTags;
                    foreach (QString arg, msg.arguments) {
                        stream << "," << arg;
                    }
                    stream << "\n";
                }
                file.close();
            }
        }
    }
}

void MainWindow::onShowTimestampsChecked(int state)
{
    showTimestamps = state ? true : false;
    updateLogView();
}

void MainWindow::onShowOnlyUpdatedOscAddresses(int state)
{
    showOnlyUpdatedAddresses = state ? true : false;
    updateLogView();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // Save settings on exit
    saveSettings();

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
