#include <QDebug>

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
                    ReceivedOscMessage *m = new ReceivedOscMessage;
                    m->port = this->port;
                    m->address = QString::fromStdString(msg->addressPattern());
                    m->typeTags = QString::fromStdString(msg->typeTags());
                    m->typeTags.insert(0, ',');

                    oscpkt::Message::ArgReader arg(msg->arg());
                    while (arg.nbArgRemaining()) {
                        if (arg.isBlob()) {
                            // TODO
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
