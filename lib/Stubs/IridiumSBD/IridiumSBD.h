/*
IridiumSBD Stub, with minimal functionality

*/
#ifndef STUB_IRIDIUM_SBD
#define STUB_IRIDIUM_SBD

#define ISBD_CLEAR_MO			0

// fake stream, doesn't do anthing will fall over if any methods called
typedef struct {
   char label;
} Stream;

class IridiumSBD
{
    public:
       IridiumSBD();
       IridiumSBD(Stream &str, int sleepPinNo = -1, int ringPinNo = -1);

       int begin();
       int sendSBDText(const char *message);
       int sendSBDBinary(const uint8_t *txData, uint16_t txDataSize);
       int getSystemTime(struct tm &tm);
       bool isAsleep();
       int sleep();

       int clearBuffers(int buffers = ISBD_CLEAR_MO);
       int getIMEI(char *IMEI, uint16_t bufferSize);

       // Weak functions to configure and set the sleep pin - user can overwrite with a custom functions if required
       void configureSleepPin() __attribute__((weak));
       void setSleepPin(uint8_t enable) __attribute__((weak));

       // Weak functions to begin and end the Serial port after power(true) and before power(false)
       void beginSerialPort() __attribute__((weak));
       void endSerialPort() __attribute__((weak));

    private:
       Stream * stream; // Communicating with the Iridium
};

#endif
