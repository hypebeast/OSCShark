#include "osclistenercontroller.h"

OscListenerController::OscListenerController(int port)
{
    workerThread = new QThread;
    worker = new OscListenerWorker(port);
    worker->moveToThread(workerThread);
    connect(workerThread, SIGNAL(started()), worker, SLOT(doWork()));

    running = false;
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
    // TODO
    if (running) {
        workerThread->quit();
        workerThread->wait();
        running = false;
    }
}
