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
    void Stop();

public slots:
    void Start();

signals:
    void finished();
    void messageReceived(ReceivedOscMessage *msg);

private:
    void doWork();
    int port;
    bool running;
    oscpkt::UdpSocket socket;
};

#endif // OSCLISTENERWORKER_H
