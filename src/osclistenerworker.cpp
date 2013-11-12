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

#include <QDebug>
#include <QDateTime>

#include "osclistenerworker.h"
#include "common.h"

OscListenerWorker::OscListenerWorker(int port)
    : port(port)
{
    this->running = false;
}

void OscListenerWorker::Start()
{
    if (!running) {
        running = true;
        doWork();
    }
}

void OscListenerWorker::Stop()
{
    if (running) {
        running = false;
        this->socket.close();
    }
}

void OscListenerWorker::doWork()
{
    this->socket.bindTo(this->port);
    if (!socket.isOk()) {
        cerr << "Error opening port " << this->port << ": " << socket.errorMessage() << "\n";
    } else {
        qDebug() << "Start listening on port: " << QString::number(this->port);

        oscpkt::PacketReader pr;
        oscpkt::PacketWriter pw;

        while (socket.isOk() && running) {
            if (socket.receiveNextPacket(50)) {
                pr.init(socket.packetData(), socket.packetSize());
                oscpkt::Message *msg;
                while (pr.isOk() && (msg = pr.popMessage()) != 0) {
                    OscMessageContainer *m = new OscMessageContainer;
                    m->port = this->port;
                    m->address = QString::fromStdString(msg->addressPattern());
                    m->typeTags = QString::fromStdString(msg->typeTags());
                    m->typeTags.insert(0, ',');
                    m->time = QDateTime::currentDateTime();

                    oscpkt::Message::ArgReader arg(msg->arg());
                    while (arg.nbArgRemaining()) {
                        if (arg.isBlob()) {
                            // TODO: Not supported
                        } else if (arg.isBool()) {
                            bool b; arg.popBool(b);
                            QString s = b ? "true" : "false";
                            m->arguments.append(s);
                        } else if (arg.isInt32()) {
                            int i; arg.popInt32(i);
                            m->arguments.append(QString::number(i));
                        } else if (arg.isInt64()) {
                            int64_t i; arg.popInt64(i);
                            m->arguments.append(QString::number(i));
                        } else if (arg.isFloat()) {
                            float f; arg.popFloat(f);
                            m->arguments.append(QString::number(f));
                        } else if (arg.isDouble()) {
                            double d; arg.popDouble(d);
                            m->arguments.append(QString::number(d));
                        } else if (arg.isStr()) {
                            std::string s; arg.popStr(s);
                            m->arguments.append(QString::fromStdString(s));
                        }
                    }

                    emit messageReceived(m);
                }
            }
        }
    }
}
