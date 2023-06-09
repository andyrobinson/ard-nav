#include "SatComm.h"

SatComm::SatComm(){};
SatComm::SatComm(IridiumSBD *modemp, Timer *timerp, Gps *gpsp, Battery *batteryp, Compass *compassp, Logger *loggerp):
    modem(modemp),timer(timerp),gps(gpsp),battery(batteryp),compass(compassp),logger(loggerp),wp_label("0"),last_modem_attempt_time(0){};

// every 15 mins, for test
// static const uint8_t log_hours[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23};
// static const uint8_t log_minutes[] = {0,15,30,15};

// every three hours, for live
const uint8_t SatComm::log_hours[8] = {0,3,6,9,12,15,18,21};
const uint8_t SatComm::log_minutes[1] = {0};

void SatComm::begin(){
    modem->sleep();
    last_log = 0; // really for testing purposes
};

bool SatComm::steer_log_or_continue() {
    bool result = true; // this is not success, this is just carry on steering

    if (!timer->isTimeSet()) tryModemTime();

    tm *t = timer->nowTm();

    if (timer->isTimeSet()
        && isHourtoLog((uint8_t) t->tm_hour, log_hours, sizeof(log_hours))
        && isMinutetoLog((uint8_t) t-> tm_min, log_minutes, sizeof(log_minutes))
        && haveNotLoggedRecently()) {

        insertLogDataIntoBuffer();
        modem->begin();
        int err = modem->sendSBDBinary(send_buffer, SAT_LOG_RECORD_SIZE);
        if (err == ISBD_SUCCESS) {
            last_log = timer->milliseconds(); // prevent retry after success
        } else if (err == ISBD_CANCELLED) {
            result = false;
        }
        modem->sleep();
    } else {
        modem->resetSBDRetry();
    }
    return result;
}

void SatComm::insertLogDataIntoBuffer() {
        stuff(timer->now(),send_buffer,0,4);
        gps->data(SAT_GPS_WAIT_MILLIS, &gps_data);
        stuff(gps_data.fpLatitude,send_buffer,4,4);
        stuff(gps_data.fpLongitude,send_buffer,8,4);
        stuff((long) (gps_data.avg_mps * 1000),send_buffer,12,4); // scaled up as long, much easier to stuff
        stuff(gps_data.cog,send_buffer,16,2);
        send_buffer[18]=wp_label[0];
        send_buffer[19]=wp_label[1];
        stuff(battery->raw_max(),send_buffer,20,2);
        stuff(battery->raw_min(),send_buffer,22,2);
        stuff(compass->bearing(),send_buffer,24,2);
        send_buffer[26] = (uint8_t) constrain(compass->err_percent(),0,100);
        stuff(dispFreeMemory(),send_buffer,27,4);
        stuff(timer->lastRestart,send_buffer,31,4);
}

void SatComm::set_dest(char *label) {
    wp_label[0] = charOrSpace(label[0]);
    wp_label[1] = charOrSpace(label[1]);
}

char SatComm::charOrSpace(char ch) {
    return ch == '\0' ? ' ' : ch;
}

bool SatComm::isMinutetoLog(uint8_t val, const uint8_t *arr, int length) {
    for (int i=0;i<length;i++) {
        if (arr[i] <= val && (arr[i] + SAT_LOG_WINDOWS_MINS) >= val) return true;
    }
    return false;
}

bool SatComm::isHourtoLog(uint8_t val, const uint8_t *arr, int length) {
    for (int i=0;i<length;i++) {
        if (arr[i] == val) return true;
    }
    return false;
}

bool SatComm::haveNotLoggedRecently() {
    return (last_log == 0
            || ((timer->milliseconds() - last_log) > (SAT_LOG_WINDOWS_MINS * SAT_MILLIS_IN_MINUTE)));
}

void SatComm::tryModemTime() {
    struct tm t;
    if ((timer->milliseconds() - last_modem_attempt_time) > (SAT_MILLIS_IN_MINUTE * SAT_TIME_SET_INTERVAL_MINS)) {
        last_modem_attempt_time = timer->milliseconds();
        modem->begin();
        int err = modem->getSystemTime(t);
        if (err == ISBD_SUCCESS) {
            timer->setTime(mktime(&t));
        }
        modem->sleep();
    }
}
