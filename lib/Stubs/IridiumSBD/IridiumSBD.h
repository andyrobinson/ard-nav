/*
IridiumSBD Stub, with minimal functionality

*/

#include "time.h"
#include "cstring"

#ifndef STUB_IRIDIUM_SBD
#define STUB_IRIDIUM_SBD

#define ISBD_CLEAR_MO			 0

// response codes
#define ISBD_SUCCESS             0
#define ISBD_ALREADY_AWAKE       1
#define ISBD_SERIAL_FAILURE      2
#define ISBD_PROTOCOL_ERROR      3
#define ISBD_CANCELLED           4
#define ISBD_NO_MODEM_DETECTED   5
#define ISBD_SBDIX_FATAL_ERROR   6
#define ISBD_SENDRECEIVE_TIMEOUT 7
#define ISBD_RX_OVERFLOW         8
#define ISBD_REENTRANT           9
#define ISBD_IS_ASLEEP           10
#define ISBD_NO_SLEEP_PIN        11
#define ISBD_NO_NETWORK          12
#define ISBD_MSG_TOO_LONG        13


class IridiumSBD
{
    public:
       IridiumSBD();

       int begin();
       int sendSBDText(const char *message);
       int sendSBDBinary(const uint8_t *txData, uint16_t txDataSize);
       int getSystemTime(struct tm &tm);
       bool isAsleep();
       int sleep();
       void resetSBDRetry();

       int clearBuffers(int buffers = ISBD_CLEAR_MO);
       int getIMEI(char *IMEI, uint16_t bufferSize);

       // stub methods for test
       void reset();
       void set_response(int code);
       void set_time(time_t t);

       int send_attempts;
       unsigned char sent[500];
       int sent_length;
       int retry_reset_count;

    private:
        bool sleeping;
        int response;
        time_t sat_time;
};

#endif
