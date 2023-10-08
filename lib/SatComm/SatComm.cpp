#include "SatComm.h"

SatComm::SatComm(){};
SatComm::SatComm(IridiumSBD *modemp, Timer *timerp, Gps *gpsp, Battery *batteryp, Compass *compassp, Logger *loggerp):
    modem(modemp),timer(timerp),gps(gpsp),battery(batteryp),compass(compassp),logger(loggerp),wp_label(' '),last_modem_attempt_time(0){};

// every 15 mins, for test
const uint8_t SatComm::log_hours[] = {6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21};
const uint8_t SatComm::log_minutes[] = {0,15,30,45};

// every 1 hour, for longer test voyages
//const uint8_t SatComm::log_hours[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23};
//const uint8_t SatComm::log_minutes[] = {0};

// every three hours, for live
// const uint8_t SatComm::log_hours[] = {0,3,6,9,12,15,18,21};
// const uint8_t SatComm::log_minutes[] = {0};

void SatComm::begin(){
    modem->sleep();
    last_log = 0; // really for testing purposes
    last_attempt = 0;
};

bool SatComm::steer_log_or_continue() {
    char logmsg[50];
    int err;

    if (!timer->isTimeSet()) tryModemTime();
    tm *t = timer->nowTm();

    // check if it's within the logging window
    if (!(timer->isTimeSet()
        && isHourtoLog((uint8_t) t->tm_hour, log_hours, sizeof(log_hours))
        && isMinutetoLog((uint8_t) t-> tm_min, log_minutes, sizeof(log_minutes))
        && noRecentSuccess())) {

        modem->resetSBDRetry();
        if (!modem->isAsleep()) {
            logger->msg("Sat sleep");
            modem->sleep();
        }
        return true;
    }

    // check for a recent attempt to log
    if (recentlyAttemptedToLog()) return true;

    if (modem->isAsleep()) {
        err = modem->begin();
        sprintf(logmsg,"Sat begin %d", err);logger->msg(logmsg);
        if (err != ISBD_SUCCESS) return true;
    }

    logger->msg("Sat log attempt");
    last_attempt = timer->milliseconds();

    insertLogDataIntoBuffer();

    modem->adjustSendReceiveTimeout(mins_left_in_window((uint8_t) t-> tm_min, log_minutes, sizeof(log_minutes)) * 60);
    err = modem->sendSBDBinary(send_buffer, SAT_LOG_RECORD_SIZE);
    sprintf(logmsg,"Sat send %d", err);logger->msg(logmsg);

    if (err == ISBD_SUCCESS) {
        logger->banner("Sat logged");
        modem->sleep();
        last_log = timer->milliseconds(); // prevent retry after success
        return true;
    }

    if (err == ISBD_CANCELLED) {
        logger->msg("Sat cancel");
        return false;
    }

    return true;  // other possible results
}

void SatComm::insertLogDataIntoBuffer() {
        stuff(timer->now(),send_buffer,0,4);
        gps->data(SAT_GPS_WAIT_MILLIS, &gps_data);
        stuff(gps_data.fpLatitude,send_buffer,4,4);
        stuff(gps_data.fpLongitude,send_buffer,8,4);
        stuff((long) (gps_data.avg_mps * 1000),send_buffer,12,4); // scaled up as long, much easier to stuff
        stuff(gps_data.cog,send_buffer,16,2);
        send_buffer[18]=wp_label;
        stuff(battery->raw_max(),send_buffer,19,2);
        stuff(battery->raw_min(),send_buffer,21,2);
        stuff(compass->bearing(),send_buffer,23,2);
        send_buffer[25] = (uint8_t) constrain(compass->err_percent(),0,100);
        stuff(dispFreeMemory(),send_buffer,26,4);
        stuff(timer->lastRestart,send_buffer,30,4);
}

void SatComm::set_dest(char label) {
    wp_label = label;
}

char SatComm::charOrSpace(char ch) {
    return ch == '\0' ? ' ' : ch;
}

bool SatComm::isMinutetoLog(uint8_t val, const uint8_t *arr, int length) {
    return mins_left_in_window(val, arr, length) > 0;
}

bool SatComm::isHourtoLog(uint8_t val, const uint8_t *arr, int length) {
    for (int i=0;i<length;i++) {
        if (arr[i] == val) return true;
    }
    return false;
}

bool SatComm::noRecentSuccess() {
    return (last_log == 0
            || ((timer->milliseconds() - last_log) > (SAT_LOG_WINDOWS_MINS * SAT_MILLIS_IN_MINUTE)));
}

bool SatComm::recentlyAttemptedToLog() {
    return (timer->milliseconds() - last_attempt) < (modem->getSBDRetryInterval() * 1000);
}

int SatComm::mins_left_in_window(uint8_t val, const uint8_t *arr, int length) {
    for (int i=0;i<length;i++) {
        int window_end = arr[i] + SAT_LOG_WINDOWS_MINS;
        if (arr[i] <= val && window_end >= val) return (window_end - val);
    }
    return 0;
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
