#ifndef OSCLISTENERCONTROLLER_H
#define OSCLISTENERCONTROLLER_H

#include <QObject>
#include <QThread>

#include "ip/UdpSocket.h"
#include "osclistenerworker.h"

class OscListenerController : QObject
{
    Q_OBJECT

public:
    OscListenerController(int);
    void Start();
    void Stop();

private:
    QThread *workerThread;
    bool running;
    OscListenerWorker *worker;
    int port;
};

#endif // OSCLISTENERCONTROLLER_H
