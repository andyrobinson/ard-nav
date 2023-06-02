#include "Gps.h"
#include "Adafruit_GPS.h"
#include "wiring_private.h"

using namespace Utility;

// not in the class, must be static

Uart Serial2(&sercom1, PIN_SERIAL2_RX, PIN_SERIAL2_TX, SERCOM_RX_PAD_3, UART_TX_PAD_2);

void SERCOM1_Handler()
{
  Serial2.IrqHandler();
}

Adafruit_GPS AGPS(&Serial2);
float avg_speed = MIN_SPEED;

void TC5_Handler(void) {
  AGPS.read();
  TC5->COUNT16.INTFLAG.bit.MC0 = 1; //reeanble interrupt
}

// class based stuff
Gps::Gps(Timer *timerp): timer(timerp) {}

void Gps::begin() {
  pinPeripheral(PIN_SERIAL2_TX, PIO_SERCOM);
  pinPeripheral(PIN_SERIAL2_RX, PIO_SERCOM);

  AGPS.begin(9600);
  AGPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGAGSA);  // with fix quality, satellites, and error values
  AGPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate

  // start the timer based interrupt
  tcConfigure(INTERRUPT_SAMPLE_RATE_MICRO_SECS);
  tcStartCounter();
}

void Gps::data(uint32_t max_millis, gpsResult *result) {
  uint32_t start_time = millis();

  do {
    //reading is now done by the timer based interrupt

    AGPS.pause(true); // stop reading data, timeout still happens

    result->fix = FIX_UNPARSEABLE_DATA;
    if (AGPS.newNMEAreceived() && !AGPS.parse(AGPS.lastNMEA())) {
      AGPS.pause(false);
      continue;
    }

    result->fix = FIX_NONE;
    result->mps = 0.0;
    result->unixTime = unix_time(AGPS.year, AGPS.month, AGPS.day, AGPS.hour, AGPS.minute, AGPS.seconds);
    timer->setTime(result->unixTime);

    if (AGPS.fix) {
      result->pos.latitude = AGPS.latitudeDegrees;
      result->pos.longitude = AGPS.longitudeDegrees;
      result->fpLatitude = AGPS.latitude_fixed;
      result->fpLongitude = AGPS.longitude_fixed;
      result->pos.error = AGPS.PDOP * MAX_ACCURACY_METRES;

      result->mps = max1(MIN_SPEED, min1(AGPS.speed * KNOTS_TO_METRES_PER_SEC, MAX_POSSIBLE_SPEED));
      avg_speed = (0.9 * avg_speed) + (0.1 * result-> mps); // 10 point moving average
      result->avg_mps = avg_speed;

      result->fix = AGPS.fixquality;
    }

    AGPS.pause(false);

    timer->wait(1); // wait a little and also reset the Watchdog

  } while (!AGPS.fix && ((millis() - start_time) < max_millis));

}

//Thanks to https://gist.github.com/nonsintetic/ad13e70f164801325f5f552f84306d6f for interrupt code

 // because of arithmetic, this is used to divide clock rate (48MHz) and produce a 16 bit value
 // so result must be less than 65535, and therefore this value must be greater than 734
 // we can adjust the prescaler to get a faster interrupt
 void Gps::tcConfigure(int sampleRate) {
   // Enable GCLK for TCC2 and TC5 (timer counter input clock)
   GCLK->CLKCTRL.reg = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID(GCM_TC4_TC5));
   while (GCLK->STATUS.bit.SYNCBUSY);

   tcReset(); //reset TC5

   // Set Timer counter Mode to 16 bits
   TC5->COUNT16.CTRLA.reg |= TC_CTRLA_MODE_COUNT16;
   // Set TC5 mode as match frequency
   TC5->COUNT16.CTRLA.reg |= TC_CTRLA_WAVEGEN_MFRQ;
   //set prescaler and enable TC5
   TC5->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1 | TC_CTRLA_ENABLE;
   TC5->COUNT16.CC[0].reg = (uint16_t) (SystemCoreClock / sampleRate - 1);
   while (tcIsSyncing());

   // Configure interrupt request
   NVIC_DisableIRQ(TC5_IRQn);
   NVIC_ClearPendingIRQ(TC5_IRQn);
   NVIC_SetPriority(TC5_IRQn, 0);
   NVIC_EnableIRQ(TC5_IRQn);

   // Enable the TC5 interrupt request
   TC5->COUNT16.INTENSET.bit.MC0 = 1;
   while (tcIsSyncing());
}

bool Gps::tcIsSyncing()
{
  return TC5->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY;
}

void Gps::tcStartCounter()
{
  TC5->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE; //set the CTRLA register
  while (tcIsSyncing()); //wait until snyc'd
}

void Gps::tcReset()
{
  TC5->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
  while (tcIsSyncing());
  while (TC5->COUNT16.CTRLA.bit.SWRST);
}

//disable TC5
void Gps::tcDisable()
{
  TC5->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
  while (tcIsSyncing());
}
