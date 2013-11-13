/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2013 Sebastian Ruml
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

#import <QDebug>

#include "osclistenercontroller.h"

OscListenerController::OscListenerController(int port)
    : port(port)
{
    workerThread = new QThread;
    worker = new OscListenerWorker(port);
    worker->moveToThread(workerThread);
    connect(workerThread, SIGNAL(started()), worker, SLOT(Start()));
    connect(worker, SIGNAL(messageReceived(OscMessageContainer*)), this, SLOT(handleMessage(OscMessageContainer*)));

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

void OscListenerController::handleMessage(OscMessageContainer *msg)
{
    emit messageReceived(msg);
}
