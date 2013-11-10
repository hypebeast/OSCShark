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
