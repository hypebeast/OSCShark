#ifndef COMMON_H
#define COMMON_H

#include <QDateTime>

typedef struct _receivedOscMessage {
   QString address;
   QString typeTags;
   QList<QString> arguments;
   QDateTime time;
   int port;
} OscMessageContainer;

#endif // COMMON_H
