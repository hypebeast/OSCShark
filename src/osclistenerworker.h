#ifndef OSCLISTENERWORKER_H
#define OSCLISTENERWORKER_H

#include <QObject>

#include "ip/UdpSocket.h"

class OscListenerWorker : public QObject
{
    Q_OBJECT

public:
    OscListenerWorker(int);

public slots:
    void doWork();
    void onMessageReceived();

signals:
    void finished();
    void messageReceived();

private:
    int port;
    UdpListeningReceiveSocket *receiveSocket;
};

#endif // OSCLISTENERWORKER_H
