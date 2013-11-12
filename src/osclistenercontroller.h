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

#ifndef OSCLISTENERCONTROLLER_H
#define OSCLISTENERCONTROLLER_H

#include <QObject>
#include <QThread>

#include "osclistenerworker.h"
#include "common.h"

class OscListenerController : public QObject
{
    Q_OBJECT

public:
    OscListenerController(int);
    void Start();
    void Stop();
    int Port();

signals:
    void messageReceived(OscMessageContainer *);

private slots:
    void handleMessage(OscMessageContainer*);

private:
    QThread *workerThread;
    OscListenerWorker *worker;
    bool running;
    int port;
};

#endif // OSCLISTENERCONTROLLER_H
