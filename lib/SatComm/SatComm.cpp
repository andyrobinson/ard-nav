#include "SatComm.h"

SatComm::SatComm(){};
SatComm::SatComm(IridiumSBD *modemp, Timer *timerp, Gps *gpsp, Logger *loggerp):
    modem(modemp),timer(timerp),gps(gpsp),logger(loggerp){};

// every 15 mins, for test
// static const uint8_t log_hours[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23};
// static const uint8_t log_minutes[] = {0,15,30,15};

// every three hours, for live
const uint8_t SatComm::log_hours[8] = {0,3,6,9,12,15,18,21};
const uint8_t SatComm::log_minutes[1] = {0};

void SatComm::begin(){
    modem->sleep();
};

bool SatComm::steer_log_or_continue() {
    tm *t = timer->nowTm();
    if (inArray((uint8_t) t->tm_hour, log_hours, sizeof(log_hours))
        && inWindow((uint8_t) t-> tm_min, log_minutes, sizeof(log_minutes))) {

        gps->data(SAT_GPS_WAIT_MILLIS, &gps_data);
        stuff(gps_data.fpLatitude,send_buffer,0,4);
        stuff(gps_data.fpLongitude,send_buffer,4,4);

        int err = modem->begin();
        err = modem->sendSBDBinary(send_buffer, SAT_LOG_RECORD_SIZE);

//          if (err != ISBD_SUCCESS)
//          {
//            Serial.print(F("sendSBDText failed: error "));
//            Serial.println(err);
//            if (err == ISBD_SENDRECEIVE_TIMEOUT)
//              Serial.println(F("Try again with a better view of the sky."));
//          }

    }
    return true; // needs to reflect the callback, which will be tricky
}

bool SatComm::inWindow(uint8_t val, const uint8_t *arr, int length) {
    for (int i=0;i<length;i++) {
        if (arr[i] <= val && (arr[i] + SAT_LOG_WINDOWS_MINS) >= val) return true;
    }
    return false;
}

bool SatComm::inArray(uint8_t val, const uint8_t *arr, int length) {
    for (int i=0;i<length;i++) {
        if (arr[i] == val) return true;
    }
    return false;
}
