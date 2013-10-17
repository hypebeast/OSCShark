#ifndef OSCLISTENERWORKER_H
#define OSCLISTENERWORKER_H

#include <QObject>

#define OSCPKT_OSTREAM_OUTPUT
#include "oscpkt/oscpkt.hh"
#include "oscpkt/udp.hh"
#include "common.h"

using std::cout;
using std::cerr;


class OscListenerWorker : public QObject
{
    Q_OBJECT

public:
    OscListenerWorker(int);

public slots:
    void doWork();

signals:
    void finished();
    void messageReceived(OscMessageContainer *msg);

private:
    int port;
    oscpkt::UdpSocket socket;
};

#endif // OSCLISTENERWORKER_H
