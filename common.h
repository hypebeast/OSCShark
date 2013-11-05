#ifndef COMMON_H
#define COMMON_H

typedef struct _receivedOscMessage {
   QString address;
   QString typeTags;
   QList<QString> arguments;
   int port;
} ReceivedOscMessage;

#endif // COMMON_H
