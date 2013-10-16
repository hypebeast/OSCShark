#ifndef OSCLISTENER_H
#define OSCLISTENER_H

#include <QObject>

#include "osc/OscPacketListener.h"
#include "ip/UdpSocket.h"

class OscListener : public QObject, public osc::OscPacketListener
{
    Q_OBJECT

public:
    OscListener();

signals:
    void messageReceived();

protected:
    virtual void ProcessMessage( const osc::ReceivedMessage&, const IpEndpointName&);

};

#endif // OSCLISTENER_H
