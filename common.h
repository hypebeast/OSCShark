#ifndef COMMON_H
#define COMMON_H

typedef struct _oscMessageContainer {
   QString address;
   QString typeTags;
   QList<QString> arguments;
   int port;
} OscMessageContainer;

#endif // COMMON_H
