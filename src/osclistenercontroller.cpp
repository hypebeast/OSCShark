#import <QDebug>

#include "osclistenercontroller.h"

OscListenerController::OscListenerController(int port)
    : port(port)
{
    workerThread = new QThread;
    worker = new OscListenerWorker(port);
    worker->moveToThread(workerThread);
    connect(workerThread, SIGNAL(started()), worker, SLOT(Start()));
    connect(worker, SIGNAL(messageReceived(ReceivedOscMessage*)), this, SLOT(handleMessage(ReceivedOscMessage*)));

    running = false;
}

int OscListenerController::Port()
{
    return port;
}

void OscListenerController::Start()
{
    if (!running) {
        workerThread->start();
        running = true;
    }
}

void OscListenerController::Stop()
{
    if (running) {
        worker->Stop();
        workerThread->quit();
        workerThread->wait();
        running = false;
    }
}

void OscListenerController::handleMessage(ReceivedOscMessage *msg)
{
    emit messageReceived(msg);
}
