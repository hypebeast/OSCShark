#include "osclistener.h"

OscListener::OscListener(QString ipaddress, int port)
{
    this->ipAddress = ipaddress;
    this->port = port;
    this->running = false;
}

void OscListener::run()
{
    if (!running) {
//        UdpListeningReceiveSocket s(
//                        IpEndpointName(IpEndpointName::ANY_ADDRESS, port),
//                        this);
//        s.Run();
        running = true;
    }
}

void OscListener::ProcessMessage( const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint )
{
    (void) remoteEndpoint;

//    qDebug() << "message received";
}
