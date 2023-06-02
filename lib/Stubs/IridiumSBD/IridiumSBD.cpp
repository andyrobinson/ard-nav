/*
IridiumSBD Stub, with minimal functionality
*/
#include "IridiumSBD.h"

#define ISBD_CLEAR_MO			 0
#define ISBD_SUCCESS             0
#define ISBD_IS_ASLEEP           10

IridiumSBD::IridiumSBD():sleeping(false),send_attempts(0){};

int IridiumSBD::begin(){
    sleeping = false;
    return ISBD_SUCCESS;
};

int IridiumSBD::sendSBDText(const char *message){
    if (sleeping) return ISBD_IS_ASLEEP;
    send_attempts++;
    return ISBD_SUCCESS;
};

int IridiumSBD::sendSBDBinary(const uint8_t *txData, uint16_t txDataSize){
    if (sleeping) return ISBD_IS_ASLEEP;
    send_attempts++;
    return ISBD_SUCCESS;
};

int IridiumSBD::getSystemTime(struct tm &tm){ return ISBD_SUCCESS; };

bool IridiumSBD::isAsleep(){
    return sleeping;
};

int IridiumSBD::sleep(){
    if (sleeping) return ISBD_IS_ASLEEP;
    sleeping = true;
    return ISBD_SUCCESS;
};

void IridiumSBD::reset() {
    send_attempts = 0;
}

int IridiumSBD::clearBuffers(int buffers){ return ISBD_SUCCESS; };
int IridiumSBD::getIMEI(char *IMEI, uint16_t bufferSize){ return ISBD_SUCCESS; };


