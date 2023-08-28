#include "StubLogger.h"

StubLogger::StubLogger():waypoint(' ') {}
void StubLogger::begin() {}
void StubLogger::msg(const char *message) {
    int i;
    for (i=0; message[i] != '\0'; i++) {
        last_msg[i] = message[i];
    }
    last_msg[i]='\0';
}
void StubLogger::banner(const char *message) {
    int i;
    for (i=0; message[i] != '\0'; i++) {
        last_msg[i] = message[i];
    }
    last_msg[i]='\0';
}
void StubLogger::setdest(char destletter) {
    waypoint=destletter;
}
void StubLogger::settack(char tackletter) {}
char *StubLogger::last_message() {
    return last_msg;
}
