#include "osclistener.h"

OscListener::OscListener()
{
}

void OscListener::ProcessMessage( const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint )
{
    (void) remoteEndpoint;

    emit messageReceived();

//    qDebug() << "message received";
}
