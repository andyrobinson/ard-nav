/*
IridiumSBD Stub, with minimal functionality
*/
#include "IridiumSBD.h"
//#include "cstdio"

#define ISBD_CLEAR_MO			 0
#define ISBD_SUCCESS             0
#define ISBD_IS_ASLEEP           10

IridiumSBD::IridiumSBD():sleeping(false),send_attempts(0),response(ISBD_SUCCESS),
    retry_reset_count(0),begin_response(ISBD_SUCCESS),sbdixInterval(10){};

int IridiumSBD::begin(){
    sleeping = false;
    return begin_response;
};

int IridiumSBD::sendSBDText(const char *message) {
    sent_length = 0;
    send_attempts++;
    if (sleeping) return ISBD_IS_ASLEEP;
    if (response != ISBD_SUCCESS) return response;

    while(message[sent_length] != '\0') {
        sent[sent_length] = message[sent_length];
        sent_length++;
    }

    sent[sent_length] = '\0';
    return ISBD_SUCCESS;
};

int IridiumSBD::sendSBDBinary(const uint8_t *txData, uint16_t txDataSize){
    sent_length = 0;
    send_attempts++;
    if (sleeping) return ISBD_IS_ASLEEP;
    if (response != ISBD_SUCCESS) return response;
    for (int i=0; i < txDataSize; i++) {
        sent[i] = txData[i];
    }
    sent_length = txDataSize;
    return ISBD_SUCCESS;
};

int IridiumSBD::getSystemTime(struct tm &tm){
    if (response != ISBD_SUCCESS) return response;
    memcpy(&tm, gmtime(&sat_time), sizeof tm);
    return ISBD_SUCCESS;
};

bool IridiumSBD::isAsleep(){
    return sleeping;
};

int IridiumSBD::sleep(){
    if (sleeping) return ISBD_IS_ASLEEP;
    sleeping = true;
    return ISBD_SUCCESS;
};

void IridiumSBD::resetSBDRetry() {
    retry_reset_count++;
}

int IridiumSBD::getSBDRetryInterval() {
    return sbdixInterval;
};

void IridiumSBD::reset() {
    send_attempts = 0;
    sent_length = 0;
    for (int i=0; i < 500;i++) sent[i] = 0;
    retry_reset_count = 0;
    response = ISBD_SUCCESS;
}

void IridiumSBD::set_response(int code){
    response = code;
}

void IridiumSBD::set_begin_response(int code){
    begin_response = code;
}

void IridiumSBD::set_time(time_t t) {
    sat_time = t;
}


int IridiumSBD::clearBuffers(int buffers){ return ISBD_SUCCESS; };
int IridiumSBD::getIMEI(char *IMEI, uint16_t bufferSize){ return ISBD_SUCCESS; };


