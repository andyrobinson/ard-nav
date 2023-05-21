/*
IridiumSBD Stub, with minimal functionality
*/
#include "IridiumSBD.h"

IridiumSBD::IridiumSBD(){};
IridiumSBD::IridiumSBD(Stream &str, int sleepPinNo, int ringPinNo):stream(&str){};

int IridiumSBD::begin(){ return 0; };
int IridiumSBD::sendSBDText(const char *message){ return 0; };
int IridiumSBD::sendSBDBinary(const uint8_t *txData, uint16_t txDataSize){ return 0; };
int IridiumSBD::getSystemTime(struct tm &tm){ return 0; };
bool IridiumSBD::isAsleep(){ return false; };
int IridiumSBD::sleep(){ return 0; };

int IridiumSBD::clearBuffers(int buffers){ return 0; };
int IridiumSBD::getIMEI(char *IMEI, uint16_t bufferSize){ return 0; };


