#include <QDebug>

#include "osclistenerworker.h"
#include "osclistener.h"
#include "common.h"

OscListenerWorker::OscListenerWorker(int port)
    : port(port)
{
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

        while (socket.isOk()) {
            if (socket.receiveNextPacket(50)) {
                pr.init(socket.packetData(), socket.packetSize());
                oscpkt::Message *msg;
                while (pr.isOk() && (msg = pr.popMessage()) != 0) {
                    OscMessageContainer *m = new OscMessageContainer;
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

                        } else if (arg.isFloat()) {

                        } else if (arg.isDouble()) {

                        } else if (arg.isStr()) {

                        }
                    }

                    emit messageReceived(m);
                }
            }
        }
    }
}
