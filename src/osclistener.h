#ifndef OSCLISTENER_H
#define OSCLISTENER_H

#include <QObject>
#include <QString>
#include <QThread>

#include "osc/OscPacketListener.h"
#include "ip/UdpSocket.h"

class OscListener : public QThread, public osc::OscPacketListener
{
    Q_OBJECT

public:
    OscListener(QString ipAddress, int port);


protected:
    virtual void ProcessMessage( const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint );

private:
    virtual void run();

    QString ipAddress;
    int port;
    bool running;
};

#endif // OSCLISTENER_H
